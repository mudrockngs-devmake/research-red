// Research Red: headless mGBA runner for automated playthroughs.
//
//   rr_run ROM SCRIPT OUTDIR
//
// Plays ROM with no window, following SCRIPT one command per line, and writes
// screenshots (and the game's save, OUTDIR/test.sav) into OUTDIR.
// Built against libmgba by setup_mgba.sh; driven by smoke.py.
//
// Script commands (keys: A B SELECT START RIGHT LEFT UP DOWN R L, join with +):
//   wait N            run N frames with nothing pressed
//   press KEYS [N]    hold KEYS for N frames (default 4), then release for 4
//   hold KEYS N       hold KEYS for N frames
//   mash KEYS N       tap KEYS every 8 frames for N frames (advance text)
//   shot NAME         save OUTDIR/NAME.png
//   save PATH         write a save state;  load PATH: load one
//   pal               print the 16 sprite palettes (debugging)
//   oam               print the first 24 sprites (debugging)
//   tiles FIRST N     print N sprite tiles from FIRST (debugging)
//   # ...             comment
#include <mgba/core/core.h>
#include <mgba/core/config.h>
#include <mgba/core/log.h>
#include <mgba/core/serialize.h>
#include <mgba-util/vfs.h>
#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static struct mCore *core;
static color_t *buf;
static unsigned W, H;

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

static void frames(int keys, int n)
{
    core->setKeys(core, keys);
    for (int i = 0; i < n; i++)
        core->runFrame(core);
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
    core->reset(core);

    FILE *s = fopen(argv[2], "r");
    if (!s)
        return 4;
    char line[512];
    while (fgets(line, sizeof line, s))
    {
        char cmd[32] = {0}, a[256] = {0};
        int n = 0;
        if (line[0] == '#' || sscanf(line, "%31s %255s %d", cmd, a, &n) < 1)
            continue;
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
            fprintf(stderr, "rr_run: unknown command '%s'\n", cmd);
    }
    fclose(s);
    core->deinit(core);
    return 0;
}
