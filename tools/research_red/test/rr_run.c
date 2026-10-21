// Research Red: headless mGBA runner for automated playthroughs.
//
//   rr_run ROM SCRIPT OUTDIR
//
// Plays ROM with no window, following SCRIPT one command per line, and writes
// screenshots (and the game's save, OUTDIR/test.sav) into OUTDIR.
// Built against libmgba by setup_mgba.sh; driven by smoke.py.
// Knows nothing about any particular game: smoke.py turns names (story step,
// map, flag) into the raw addresses used below.
//
// Script commands (keys: A B SELECT START RIGHT LEFT UP DOWN R L, join with +):
//   wait N            run N frames with nothing pressed
//   press KEYS [N]    hold KEYS for N frames (default 4), then release for 4
//   hold KEYS N       hold KEYS for N frames
//   mash KEYS N       tap KEYS every 8 frames for N frames (advance text)
//   shot NAME         save OUTDIR/NAME.png
//   save PATH         write a save state;  load PATH: load one
//   check LABEL COND           print "CHECK LABEL pass|fail value"
//   until LABEL MAX KEYS COND  run until COND is true (tapping KEYS, or - for
//                              none), at most MAX frames; prints "UNTIL LABEL
//                              ok|timeout frames" and shoots timeout_LABEL.png
//   autoshot N [PREFIX]  during "until", save a screenshot every N frames
//                     (PREFIX0001, PREFIX0002, ...; default prefix s; 0 = off)
//   peek LABEL ADDR   print "PEEK LABEL value"
//   stuck N           report "stuck" after N unchanged frames (0 = off;
//                     default 1200, 20 seconds)
//   idle COND         the screen may legitimately sit still while COND is
//                     true (e.g. the player is free on the map); each idle
//                     line adds one such case. "idle clear" forgets them all.
//   pal               print the 16 sprite palettes (debugging)
//   oam               print the first 24 sprites (debugging)
//   tiles FIRST N     print N sprite tiles from FIRST (debugging)
//   # ...             comment
//
// Addresses: 0x0203A000 reads there; *0x03004F70+0x2F62 reads the pointer at
// 0x03004F70 and adds the offset. Add :1 :2 or :4 for the size (default 1).
// COND: ADDR OP VALUE, OP one of == != < > <= >= & (any of these bits set)
// !& (none set); join several with && (all must hold), and groups of those
// with || (any group may hold; && binds tighter).
//
// Clock: the game's clock is fake, not the computer's. Every run starts at
// 2026-10-20 10:00 (morning) and the clock moves with the emulated frames, so
// a run plays out the same at any time of day (the Rest menu, day/night and
// anything random seeded from the clock). Save states keep their clock.
//
// Stuck detection: if the screen stays identical for N frames while buttons
// are being pressed (and no idle case applies), it prints "STUCK line frame
// lastshot" and saves stuck_LINE.png, once per frozen stretch.
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FAKE_CLOCK_START 1792490400LL // 2026-10-20 10:00:00 UTC

static struct mCore *core;
static color_t *buf;
static unsigned W, H;
static const char *sOutDir;

// Stuck detection
static unsigned long sFrame;         // frames run so far
static int sLine;                    // current script line
static char sLastShot[256] = "-";    // last screenshot taken
static int sStuckLimit = 1200;
static int sStill;                   // frames the screen hasn't changed
static int sPressedWhileStill;       // buttons pressed during that stretch
static int sStuckReported;
static uint32_t sLastHash;
static int sAutoShot;                // "until" screenshot interval (0 = off)
static char sAutoPrefix[64] = "s";
static int sAutoCount;

#define MAX_TERMS 10
#define MAX_IDLE 8
struct Term
{
    uint32_t ptr, off; // ptr != 0: read the pointer at ptr, then add off
    int size;
    char op[3];
    uint32_t value;
    int orBefore;      // a "||" came before this term: it starts a new group
};
struct Cond
{
    int n;
    struct Term t[MAX_TERMS];
};
static struct Cond sIdle[MAX_IDLE];
static int sIdleCount;

static int keymask(const char *s)
{
    static const char *const names[] = {"A", "B", "SELECT", "START", "RIGHT", "LEFT", "UP", "DOWN", "R", "L"};
    int m = 0;
    char tmp[128];
    strncpy(tmp, s, sizeof tmp - 1);
    tmp[sizeof tmp - 1] = 0;
    for (char *t = strtok(tmp, "+"); t; t = strtok(NULL, "+"))
        for (int i = 0; i < 10; i++)
            if (!strcmp(t, names[i]))
                m |= 1 << i;
    return m;
}

static void shot(const char *path);

// --- memory ---------------------------------------------------------------
// "0x0203A000:2" or "*0x03004F70+0x2F62:2"
static int parseAddr(const char *s, struct Term *t)
{
    char *end;
    t->ptr = 0;
    t->size = 1;
    if (*s == '*')
    {
        t->ptr = strtoul(s + 1, &end, 0);
        t->off = *end == '+' ? strtoul(end + 1, &end, 0) : 0;
    }
    else
        t->off = strtoul(s, &end, 0);
    if (*end == ':')
        t->size = atoi(end + 1);
    return t->size == 1 || t->size == 2 || t->size == 4;
}

static uint32_t readAddr(const struct Term *t)
{
    uint32_t a = t->off;
    if (t->ptr)
        a += core->rawRead32(core, t->ptr, -1);
    if (t->size == 4)
        return core->rawRead32(core, a, -1);
    if (t->size == 2)
        return core->rawRead16(core, a, -1);
    return core->rawRead8(core, a, -1);
}

static int termHolds(const struct Term *t)
{
    uint32_t v = readAddr(t);
    if (!strcmp(t->op, "==")) return v == t->value;
    if (!strcmp(t->op, "!=")) return v != t->value;
    if (!strcmp(t->op, "<")) return v < t->value;
    if (!strcmp(t->op, ">")) return v > t->value;
    if (!strcmp(t->op, "<=")) return v <= t->value;
    if (!strcmp(t->op, ">=")) return v >= t->value;
    if (!strcmp(t->op, "&")) return (v & t->value) != 0;
    if (!strcmp(t->op, "!&")) return (v & t->value) == 0;
    return 0;
}

static int condHolds(const struct Cond *c)
{
    int group = 1; // the current && group holds so far
    for (int i = 0; i < c->n; i++)
    {
        if (c->t[i].orBefore)
        {
            if (group)
                return 1;
            group = 1;
        }
        if (group && !termHolds(&c->t[i]))
            group = 0;
    }
    return group;
}

// tok[0..] = ADDR OP VALUE [&&|"||" ADDR OP VALUE ...]
static int parseCond(char **tok, int ntok, struct Cond *c)
{
    c->n = 0;
    for (int i = 0; i + 2 < ntok && c->n < MAX_TERMS; i += 4)
    {
        struct Term *t = &c->t[c->n++];
        t->orBefore = i > 0 && !strcmp(tok[i - 1], "||");
        if (!parseAddr(tok[i], t))
            return 0;
        strncpy(t->op, tok[i + 1], 2);
        t->op[2] = 0;
        t->value = strtoul(tok[i + 2], NULL, 0);
        if (i + 3 < ntok && strcmp(tok[i + 3], "&&") && strcmp(tok[i + 3], "||"))
            return 0;
    }
    return c->n > 0;
}

// --- running frames ---------------------------------------------------------
static uint32_t screenHash(void)
{
    uint32_t h = 2166136261u;
    for (unsigned i = 0; i < W * H; i++)
        h = (h ^ (uint32_t)buf[i]) * 16777619u;
    return h;
}

static void watchStuck(int keys)
{
    uint32_t h = screenHash();
    if (h != sLastHash)
    {
        sLastHash = h;
        sStill = 0;
        sPressedWhileStill = 0;
        sStuckReported = 0;
        return;
    }
    sStill++;
    if (keys)
        sPressedWhileStill = 1;
    if (!sStuckLimit || sStuckReported || sStill < sStuckLimit || !sPressedWhileStill)
        return;
    for (int i = 0; i < sIdleCount; i++)
        if (condHolds(&sIdle[i]))
            return;
    char p[1024];
    snprintf(p, sizeof p, "%s/stuck_%05d.png", sOutDir, sLine);
    shot(p);
    printf("STUCK %d %lu %s\n", sLine, sFrame, sLastShot);
    fflush(stdout);
    sStuckReported = 1;
}

static void frames(int keys, int n)
{
    core->setKeys(core, keys);
    for (int i = 0; i < n; i++)
    {
        core->runFrame(core);
        sFrame++;
        watchStuck(keys);
    }
}

static void shot(const char *path)
{
    FILE *f = fopen(path, "wb");
    if (!f)
        return;
    png_structp p = png_create_write_struct(PNG_LIBPNG_VER_STRING, 0, 0, 0);
    png_infop info = png_create_info_struct(p);
    png_init_io(p, f);
    png_set_IHDR(p, info, W, H, 8, PNG_COLOR_TYPE_RGB, 0, 0, 0);
    png_write_info(p, info);
    unsigned char *row = malloc(W * 3);
    for (unsigned y = 0; y < H; y++)
    {
        for (unsigned x = 0; x < W; x++)
        {
            uint32_t c = buf[y * W + x];
            row[x * 3] = c & 0xFF;
            row[x * 3 + 1] = (c >> 8) & 0xFF;
            row[x * 3 + 2] = (c >> 16) & 0xFF;
        }
        png_write_row(p, row);
    }
    png_write_end(p, info);
    png_destroy_write_struct(&p, &info);
    fclose(f);
    free(row);
}

static void quietLog(struct mLogger *l, int c, enum mLogLevel lv, const char *f, va_list a)
{
    (void)l; (void)c; (void)lv; (void)f; (void)a;
}
static struct mLogger sQuiet = {.log = quietLog};

int main(int argc, char **argv)
{
    if (argc < 4)
    {
        fprintf(stderr, "usage: rr_run ROM SCRIPT OUTDIR\n");
        return 1;
    }
    mLogSetDefaultLogger(&sQuiet);
    core = mCoreFind(argv[1]);
    if (!core)
        return 2;
    core->init(core);
    mCoreInitConfig(core, NULL);
    core->desiredVideoDimensions(core, &W, &H);
    buf = malloc(W * H * sizeof(color_t));
    core->setVideoBuffer(core, buf, W);
    if (!mCoreLoadFile(core, argv[1]))
        return 3;
    char sav[1024];
    snprintf(sav, sizeof sav, "%s/test.sav", argv[3]);
    mCoreLoadSaveFile(core, sav, false);
    setenv("TZ", "UTC", 1);   // the fake clock reads the same on any machine
    tzset();
    core->rtc.override = RTC_FAKE_EPOCH;
    core->rtc.value = FAKE_CLOCK_START * 1000;
    core->reset(core);

    sOutDir = argv[3];
    FILE *s = fopen(argv[2], "r");
    if (!s)
        return 4;
    char line[1024];
    while (fgets(line, sizeof line, s))
    {
        sLine++;
        char *tok[32];
        int ntok = 0;
        for (char *t = strtok(line, " \t\r\n"); t && ntok < 32; t = strtok(NULL, " \t\r\n"))
            tok[ntok++] = t;
        if (!ntok || tok[0][0] == '#')
            continue;
        const char *cmd = tok[0];
        const char *a = ntok > 1 ? tok[1] : "";
        int n = ntok > 2 ? atoi(tok[2]) : 0;
        if (!strcmp(cmd, "wait"))
            frames(0, atoi(a));
        else if (!strcmp(cmd, "press"))
        {
            frames(keymask(a), n ? n : 4);
            frames(0, 4);
        }
        else if (!strcmp(cmd, "hold"))
            frames(keymask(a), n);
        else if (!strcmp(cmd, "mash"))
            for (int i = 0; i < n; i += 8)
            {
                frames(keymask(a), 2);
                frames(0, 6);
            }
        else if (!strcmp(cmd, "shot"))
        {
            char p[1024];
            snprintf(p, sizeof p, "%s/%s.png", argv[3], a);
            shot(p);
            snprintf(sLastShot, sizeof sLastShot, "%s", a);
        }
        else if (!strcmp(cmd, "save"))
        {
            struct VFile *vf = VFileOpen(a, O_CREAT | O_TRUNC | O_RDWR);
            if (vf)
            {
                mCoreSaveStateNamed(core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC);
                vf->close(vf);
            }
        }
        else if (!strcmp(cmd, "load"))
        {
            struct VFile *vf = VFileOpen(a, O_RDONLY);
            if (vf)
            {
                mCoreLoadStateNamed(core, vf, SAVESTATE_SAVEDATA | SAVESTATE_RTC);
                vf->close(vf);
            }
        }
        else if (!strcmp(cmd, "check") && ntok >= 5)
        {
            struct Cond c;
            if (!parseCond(tok + 2, ntok - 2, &c))
                fprintf(stderr, "rr_run: line %d: bad condition\n", sLine);
            else
                printf("CHECK %s %s %u\n", a, condHolds(&c) ? "pass" : "fail", readAddr(&c.t[0]));
        }
        else if (!strcmp(cmd, "until") && ntok >= 7)
        {
            struct Cond c;
            int max = atoi(tok[2]), keys = strcmp(tok[3], "-") ? keymask(tok[3]) : 0, waited = 0;
            if (!parseCond(tok + 4, ntok - 4, &c))
                fprintf(stderr, "rr_run: line %d: bad condition\n", sLine);
            else
            {
                while (!condHolds(&c) && waited < max)
                {
                    frames(waited % 8 < 2 ? keys : 0, 1);
                    waited++;
                    if (sAutoShot && waited % sAutoShot == 0)
                    {
                        char p[1024];
                        snprintf(sLastShot, sizeof sLastShot, "%s%04d", sAutoPrefix, ++sAutoCount);
                        snprintf(p, sizeof p, "%s/%s.png", argv[3], sLastShot);
                        shot(p);
                    }
                }
                if (condHolds(&c))
                    printf("UNTIL %s ok %d\n", a, waited);
                else
                {
                    char p[1024];
                    snprintf(p, sizeof p, "%s/timeout_%s.png", argv[3], a);
                    shot(p);
                    printf("UNTIL %s timeout %d\n", a, waited);
                }
            }
        }
        else if (!strcmp(cmd, "poke") && ntok >= 3)
        {
            // poke <addr[:size]> <value>, or poke <addr:1> |<mask> / &~<mask>
            // to set / clear bits (flags). Research Red test setup (#18).
            struct Term t;
            if (parseAddr(tok[1], &t))
            {
                uint32_t a = t.off + (t.ptr ? core->rawRead32(core, t.ptr, -1) : 0);
                uint32_t v;
                if (tok[2][0] == '|')
                    v = readAddr(&t) | strtoul(tok[2] + 1, NULL, 0);
                else if (tok[2][0] == '&' && tok[2][1] == '~')
                    v = readAddr(&t) & ~strtoul(tok[2] + 2, NULL, 0);
                else
                    v = strtoul(tok[2], NULL, 0);
                if (t.size == 4)
                    core->rawWrite32(core, a, -1, v);
                else if (t.size == 2)
                    core->rawWrite16(core, a, -1, v);
                else
                    core->rawWrite8(core, a, -1, v);
            }
        }
        else if (!strcmp(cmd, "peek") && ntok >= 3)
        {
            struct Term t;
            if (parseAddr(tok[2], &t))
                printf("PEEK %s %u\n", a, readAddr(&t));
        }
        else if (!strcmp(cmd, "autoshot"))
        {
            sAutoShot = atoi(a);
            if (ntok > 2)
                snprintf(sAutoPrefix, sizeof sAutoPrefix, "%s", tok[2]);
        }
        else if (!strcmp(cmd, "stuck"))
            sStuckLimit = atoi(a);
        else if (!strcmp(cmd, "idle") && !strcmp(a, "clear"))
            sIdleCount = 0;
        else if (!strcmp(cmd, "idle") && sIdleCount < MAX_IDLE)
        {
            if (parseCond(tok + 1, ntok - 1, &sIdle[sIdleCount]))
                sIdleCount++;
            else
                fprintf(stderr, "rr_run: line %d: bad condition\n", sLine);
        }
        else if (!strcmp(cmd, "pal"))
            for (int pl = 0; pl < 16; pl++)
            {
                printf("OBJ%2d:", pl);
                for (int i = 0; i < 16; i++)
                    printf(" %04x", core->rawRead16(core, 0x05000200 + pl * 32 + i * 2, -1));
                printf("\n");
            }
        else if (!strcmp(cmd, "oam"))
            for (int i = 0; i < 24; i++)
            {
                unsigned a0 = core->rawRead16(core, 0x07000000 + i * 8, -1);
                unsigned a1 = core->rawRead16(core, 0x07000002 + i * 8, -1);
                unsigned a2 = core->rawRead16(core, 0x07000004 + i * 8, -1);
                printf("OAM%2d y=%3u x=%3u tile=%4u pal=%u\n", i, a0 & 255, a1 & 511, a2 & 1023, a2 >> 12);
            }
        else if (!strcmp(cmd, "tiles"))
            for (int t = atoi(a); t < atoi(a) + n; t++)
            {
                printf("T%d:", t);
                for (int i = 0; i < 16; i++)
                    printf(" %04x", core->rawRead16(core, 0x06010000 + t * 32 + i * 2, -1));
                printf("\n");
            }
        else
            fprintf(stderr, "rr_run: line %d: unknown or incomplete command '%s'\n", sLine, cmd);
        fflush(stdout);
    }
    fclose(s);
    core->deinit(core);
    return 0;
}
