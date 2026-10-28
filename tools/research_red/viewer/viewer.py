#!/usr/bin/env python3
"""Research Red: the live viewer, a page in the Windows browser.

  python3 tools/research_red/viewer/viewer.py open            # start (if needed) and open the page
  python3 tools/research_red/viewer/viewer.py show FILE.png [FILE2.png ...] [--title "Which hair?"] [--note "..."]
        put a picture in the page's "Claude asks" panel (the page pops up). Options:
          --piece ow:elm           Approve / Needs work buttons; the verdict goes to the
                                   art tracker (art_gate.py verdicts)
          --choices "A,B,C"        one button per choice (the answer is the choice)
          two or more files        compare: side by side, and a before/after slider for two
          --frames 16x32 [--order 0,3,0,4] [--ms 133]
                                   play a sprite sheet's frames (game speed: 133 ms a step)
          --map MAP_X --at 9,7     the sprite (frame 0) on that map at that square, at the
                                   size it is in the game (240x160 screen)
          --wait MINUTES           wait for the reply to THIS question and print it (exit 0),
                                   or exit 2 if none came in time: a test run (or Claude)
                                   can stop mid-play, ask, and carry on with the answer
  python3 tools/research_red/viewer/viewer.py board MAP_X [--title "Where should Ivy stand?"]
        open the scene board for a map: drag people, click to turn them,
        add people from the list, then "Send to Claude"
  python3 tools/research_red/viewer/viewer.py answers         # what came back (board sends, notes)
  python3 tools/research_red/viewer/viewer.py answer STAMP    # the reply to one question (exit 2 if none yet)
  python3 tools/research_red/viewer/viewer.py stop

The stream shows every new picture from the art tools (art\\toolkit\\) and
the newest test run (test-runs\\) as they are written: pause it, click a
picture to pin it beside the stream. Only this computer can reach the page
(it listens on 127.0.0.1). It's light: the page asks for news once a second.
Replies (board sends, notes typed in the page) land in
E:\\Workspace\\romhacks\\research-red\\viewer\\answers\\ and `answers` prints them.
"""
import json
import os
import re
import shutil
import signal
import struct
import subprocess
import sys
import threading
import time
import urllib.parse
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(os.path.dirname(os.path.dirname(HERE)))
sys.path.insert(0, os.path.join(REPO, "tools/research_red/art"))
sys.path.insert(0, os.path.join(REPO, "tools/research_red"))

PORT = int(os.environ.get("RR_VIEWER_PORT", "8765"))
WORKSPACE = "/mnt/e/Workspace/romhacks/research-red"
HOME = os.path.join(WORKSPACE, "viewer")            # the viewer's own folder on E:
SHOW = os.path.join(HOME, "show")                    # pictures Claude asks about
ANSWERS = os.path.join(HOME, "answers")              # what the owner sends back
BOARDS = os.path.join(HOME, "boards")                # scene board data + map pictures
WATCH = [os.path.join(WORKSPACE, "art", "toolkit"), os.path.join(WORKSPACE, "test-runs")]
PIDFILE = os.path.join(os.path.expanduser("~/.local/state"), "rr-viewer.pid")
IMG = (".png", ".gif", ".jpg")

for d in (SHOW, ANSWERS, BOARDS):
    os.makedirs(d, exist_ok=True)


# ---------------------------------------------------------------- what's new

def newest_run():
    root = WATCH[1]
    runs = [os.path.join(root, d) for d in os.listdir(root) if os.path.isdir(os.path.join(root, d))] if os.path.isdir(root) else []
    return max(runs, key=os.path.getmtime) if runs else None


class Index:
    """Pictures in the watched folders, kept in memory and refreshed in the
    background every 2 seconds. Reading E: from Ubuntu is slow, so folders are
    only re-listed when they change; the art toolkit's few hundred files are
    also checked for being rewritten in place (previews are rebuilt)."""

    def __init__(self):
        self.files = {}      # path -> mtime
        self.listed = {}     # dir -> (mtime, [subdirs], [files])
        self.lock = threading.Lock()

    def _visit(self, d, recheck_files):
        try:
            st = os.stat(d)
        except OSError:
            return
        old = self.listed.get(d)
        if old is None or old[0] != st.st_mtime:
            subs, files = [], []
            try:
                for e in os.scandir(d):
                    if e.is_dir() and not e.name.startswith("_"):
                        subs.append(e.path)
                    elif e.name.lower().endswith(IMG):
                        files.append(e.path)
            except OSError:
                return
            self.listed[d] = (st.st_mtime, subs, files)
            for f in files:
                if f not in self.files:
                    try:
                        self.files[f] = os.path.getmtime(f)
                    except OSError:
                        pass
        _, subs, files = self.listed[d]
        if recheck_files:
            for f in files:
                try:
                    self.files[f] = os.path.getmtime(f)
                except OSError:
                    self.files.pop(f, None)
        for sub in subs:
            self._visit(sub, recheck_files)

    def refresh(self):
        with self.lock:
            self._visit(WATCH[0], True)
            run = newest_run()
            if run:
                self._visit(run, False)

    def recent(self, since, limit=60):
        with self.lock:
            found = sorted(((m, p) for p, m in self.files.items() if m > since), reverse=True)[:limit]
        return [{"path": p, "mtime": m, "name": os.path.relpath(p, WORKSPACE)} for m, p in found]


INDEX = Index()


def keep_indexing():
    while True:
        try:
            INDEX.refresh()
        except Exception as e:  # a folder vanished mid-scan: try again next round
            print("index:", e, flush=True)
        time.sleep(2)


def recent_images(since, limit=60):
    return INDEX.recent(since, limit)


def shows(limit=8):
    items = []
    for f in sorted(os.listdir(SHOW)):
        if f.endswith(".json"):
            try:
                items.append(json.load(open(os.path.join(SHOW, f))))
            except (OSError, ValueError):
                pass
    return items[-limit:] if limit else items


def history():
    """Every question Claude asked, newest first, each with the answers sent about it."""
    answers = {}
    for f in sorted(os.listdir(ANSWERS)):
        if f.endswith(".json"):
            try:
                a = json.load(open(os.path.join(ANSWERS, f)))
            except (OSError, ValueError):
                continue
            answers.setdefault(a.get("ask") or a.get("about") or "", []).append(a)
    out = []
    for s in reversed(shows(0)):
        s = dict(s)
        s["answers"] = answers.get(s.get("time"), []) + answers.get(s.get("title"), [])
        out.append(s)
    return out


def see_through(path):
    """GBA pictures keep their see-through colour as palette colour 0 (often bright
    green); make the viewer's copy show it as see-through, like the game does."""
    try:
        from PIL import Image
        im = Image.open(path)
        if im.mode == "P" and "transparency" not in im.info:
            im.save(path, transparency=0)
    except (OSError, ValueError):
        pass


def ingame_png(map_id, x, y, img_path, fw, fh):
    """A 240x160 GBA screen: the map around square (x, y) at its real size, the
    map's own people, and the sprite (frame 0) standing on that square."""
    import io
    import artlib
    import render_map
    from PIL import Image
    out = os.path.join(BOARDS, map_id + ".png")
    if not os.path.exists(out) or time.time() - os.path.getmtime(out) > 60:
        render_map.render_map(map_id, out)
    full = Image.open(out).convert("RGBA")
    full = full.resize((full.width // 2, full.height // 2), Image.NEAREST)  # 16 px squares, like the game
    m = render_map.find_map(map_id)

    def stand(frame_img, sx, sy):
        full.alpha_composite(frame_img, (sx * 16 + (16 - frame_img.width) // 2, (sy + 1) * 16 - frame_img.height))
    for o in m.get("object_events", []):
        if (o["x"], o["y"]) == (x, y):
            continue
        png = sprite_png(o.get("graphics_id", ""), movement_facing(o.get("movement_type", "")))
        if png:
            stand(Image.open(io.BytesIO(png)).convert("RGBA"), o["x"], o["y"])
    grid = artlib.load_grid(img_path)
    stand(artlib.to_rgba(artlib.crop(grid, 0, 0, min(fw, len(grid[0])), min(fh, len(grid)))), x, y)
    # the camera keeps the player a little below the middle (FireRed: 7.5 squares across, 5 down)
    left, top = x * 16 + 8 - 120, y * 16 + 8 - 80
    left = max(0, min(left, full.width - 240))
    top = max(0, min(top, full.height - 160))
    screen = Image.new("RGBA", (240, 160), (0, 0, 0, 255))
    screen.alpha_composite(full.crop((left, top, left + 240, top + 160)))
    buf = io.BytesIO()
    screen.convert("RGB").save(buf, "PNG")
    return buf.getvalue()


# ---------------------------------------------------------------- scene board data

def board_data(map_id):
    import render_map
    m = render_map.find_map(map_id)
    out_png = os.path.join(BOARDS, map_id + ".png")
    render_map.render_map(map_id, out_png)  # 2x scale: 32 px per square
    table = gfx_table()
    people = []
    for o in m.get("object_events", []):
        people.append({"id": o.get("local_id", ""), "gfx": o.get("graphics_id", ""), "x": o["x"], "y": o["y"],
                       "facing": movement_facing(o.get("movement_type", "")), "script": o.get("script", "")})
    # people anyone can add: the story cast first, then everyone with a sprite
    cast = [k for k in table if k.startswith("OBJ_EVENT_GFX_RR_")] + \
        ["OBJ_EVENT_GFX_MOM", "OBJ_EVENT_GFX_PROF_OAK", "OBJ_EVENT_GFX_DAISY", "OBJ_EVENT_GFX_BROCK", "OBJ_EVENT_GFX_MISTY"]
    others = sorted(k for k in table if k not in cast and table[k][1] == 16 and table[k][2] == 32)
    return {"map": map_id, "image": "/file?path=" + urllib.parse.quote(out_png), "people": people,
            "cast": cast, "others": others}


def movement_facing(mt):
    for word, d in (("UP", "up"), ("NORTH", "up"), ("LEFT", "left"), ("WEST", "left"), ("RIGHT", "right"), ("EAST", "right")):
        if word in mt:
            return d
    return "down"


_TABLE = {"t": 0, "table": {}}


def gfx_table():
    """library.gfx_table(), re-read at most every 30 seconds (it parses source files)."""
    import library
    if time.time() - _TABLE["t"] > 30:
        _TABLE["table"], _TABLE["t"] = library.gfx_table(), time.time()
    return _TABLE["table"]


def sprite_png(gfx, facing):
    """One standing frame of a graphic as PNG bytes (right = left mirrored)."""
    import io
    import artlib
    idx = {"down": 0, "up": 1, "left": 2, "right": 2}.get(facing, 0)
    m = re.match(r"OBJ_EVENT_GFX_SPECIES\((\w+)\)", gfx)
    if m:
        path, w, h = os.path.join(REPO, "graphics/pokemon", m.group(1).lower(), "overworld.png"), 32, 32
    else:
        table = gfx_table()
        if gfx not in table:
            return None
        path, w, h = os.path.join(REPO, table[gfx][0]), table[gfx][1], table[gfx][2]
    if not os.path.exists(path):
        return None
    grid = artlib.load_grid(path)
    fw = min(w, len(grid[0]))
    idx = min(idx, len(grid[0]) // fw - 1)
    frame = artlib.crop(grid, idx * fw, 0, fw, min(h, len(grid)))
    if facing == "right":
        frame = artlib.mirror(frame)
    im = artlib.to_rgba(frame)
    buf = io.BytesIO()
    im.save(buf, "PNG")
    return buf.getvalue()


# ---------------------------------------------------------------- server

class Handler(BaseHTTPRequestHandler):
    def log_message(self, *a):
        pass

    def send(self, code, body, ctype="application/json"):
        try:
            self._send(code, body, ctype)
        except (BrokenPipeError, ConnectionResetError):
            pass  # the page moved on before the answer arrived

    def _send(self, code, body, ctype):
        if isinstance(body, (dict, list)):
            body = json.dumps(body).encode()
        elif isinstance(body, str):
            body = body.encode()
        self.send_response(code)
        self.send_header("Content-Type", ctype)
        self.send_header("Cache-Control", "no-store")
        self.end_headers()
        self.wfile.write(body)

    def do_GET(self):
        u = urllib.parse.urlparse(self.path)
        q = urllib.parse.parse_qs(u.query)
        if u.path in ("/", "/index.html"):
            return self.send(200, open(os.path.join(HERE, "stream.html"), "rb").read(), "text/html; charset=utf-8")
        if u.path == "/board":
            return self.send(200, open(os.path.join(HERE, "board.html"), "rb").read(), "text/html; charset=utf-8")
        if u.path == "/api/ping":
            return self.send(200, {"ok": True})
        if u.path == "/api/news":
            since = float(q.get("since", ["0"])[0])
            return self.send(200, {"now": time.time(), "images": recent_images(since), "shows": shows()})
        if u.path == "/api/history":
            return self.send(200, history())
        if u.path == "/api/ingame":
            try:
                if not os.path.realpath(q["img"][0]).startswith(os.path.realpath(HOME) + os.sep):
                    return self.send(403, {"error": "not allowed"})
                png = ingame_png(q["map"][0], int(q["x"][0]), int(q["y"][0]), q["img"][0],
                                 int(q.get("fw", ["16"])[0]), int(q.get("fh", ["32"])[0]))
                return self.send(200, png, "image/png")
            except (SystemExit, KeyError, ValueError, OSError, StopIteration) as e:
                return self.send(404, {"error": "no in-game preview: %s" % e})
        if u.path == "/api/board":
            try:
                return self.send(200, board_data(q["map"][0]))
            except (SystemExit, KeyError, StopIteration) as e:
                return self.send(404, {"error": "no map %s" % q.get("map", ["?"])[0]})
        if u.path == "/api/sprite":
            png = sprite_png(q["gfx"][0], q.get("facing", ["down"])[0])
            return self.send(200, png, "image/png") if png else self.send(404, {"error": "no sprite"})
        if u.path == "/file":
            path = os.path.realpath(q["path"][0])
            allowed = [os.path.realpath(p) for p in WATCH + [HOME]]
            if not any(path.startswith(a + os.sep) for a in allowed) or not os.path.isfile(path):
                return self.send(403, {"error": "not allowed"})
            ctype = {"png": "image/png", "gif": "image/gif", "jpg": "image/jpeg"}.get(path.rsplit(".", 1)[-1].lower(), "application/octet-stream")
            return self.send(200, open(path, "rb").read(), ctype)
        return self.send(404, {"error": "not found"})

    def do_POST(self):
        n = int(self.headers.get("Content-Length", "0"))
        data = json.loads(self.rfile.read(n) or b"{}")
        stamp = time.strftime("%Y%m%d_%H%M%S")
        # notes, verdicts (Approve / Needs work about a tracker piece), choices, board sends
        kind = "board" if self.path == "/api/board" else data.get("kind") if data.get("kind") in ("verdict", "choice") else "note"
        data["kind"], data["sent"] = kind, time.strftime("%Y-%m-%d %H:%M:%S")
        path = os.path.join(ANSWERS, "%s_%s.json" % (stamp, kind))
        json.dump(data, open(path, "w"), indent=2)
        self.send(200, {"ok": True, "saved": os.path.basename(path)})


def find_answer(stamp):
    """The newest reply to the question shown at `stamp`, or None."""
    for f in sorted(os.listdir(ANSWERS), reverse=True):
        if f.endswith(".json"):
            try:
                data = json.load(open(os.path.join(ANSWERS, f)))
            except (OSError, ValueError):
                continue
            if data.get("ask") == stamp:
                return data
    return None


def wait_answer(stamp, seconds):
    """Check for the reply every 2 seconds, up to `seconds`."""
    end = time.time() + seconds
    while time.time() < end:
        got = find_answer(stamp)
        if got:
            return got
        time.sleep(2)
    return None


def serve():
    srv = ThreadingHTTPServer(("127.0.0.1", PORT), Handler)
    threading.Thread(target=keep_indexing, daemon=True).start()
    os.makedirs(os.path.dirname(PIDFILE), exist_ok=True)
    open(PIDFILE, "w").write(str(os.getpid()))
    try:
        srv.serve_forever()
    finally:
        try:
            os.remove(PIDFILE)
        except OSError:
            pass


def running():
    import urllib.request
    try:
        urllib.request.urlopen("http://127.0.0.1:%d/api/ping" % PORT, timeout=1)
        return True
    except Exception:
        return False


def start():
    if running():
        return
    log = open(os.path.join(HOME, "viewer.log"), "a")
    # low priority (nice): the viewer never competes with a game or a video
    subprocess.Popen(["nice", "-n", "10", sys.executable, os.path.abspath(__file__), "serve"],
                     stdout=log, stderr=log, start_new_session=True)
    for _ in range(30):
        if running():
            return
        time.sleep(0.1)
    raise SystemExit("the viewer didn't start (see %s)" % os.path.join(HOME, "viewer.log"))


def open_page(path="/"):
    url = "http://localhost:%d%s" % (PORT, path)
    # cmd.exe start opens the Windows default browser
    subprocess.run(["cmd.exe", "/c", "start", "", url], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL, cwd="/mnt/c")
    print("viewer:", url)


def main(argv):
    if not argv or argv[0] in ("-h", "--help"):
        print(__doc__)
        return 0
    cmd = argv[0]
    opts = {}
    rest = []
    i = 1
    while i < len(argv):
        if argv[i].startswith("--") and i + 1 < len(argv):
            opts[argv[i][2:]] = argv[i + 1]
            i += 2
        else:
            rest.append(argv[i])
            i += 1
    if cmd == "serve":
        serve()
    elif cmd == "open":
        start()
        open_page()
    elif cmd == "show":
        start()
        stamp = time.strftime("%Y%m%d_%H%M%S")
        images, copies = [], []
        for n, src in enumerate(rest):
            dst = os.path.join(SHOW, "%s_%d_%s" % (stamp, n, os.path.basename(src)))
            shutil.copy(os.path.abspath(src), dst)
            see_through(dst)
            copies.append(dst)
            images.append({"src": "/file?path=" + urllib.parse.quote(dst), "name": os.path.basename(src)})
        meta = {"image": images[0]["src"], "images": images, "title": opts.get("title", os.path.basename(rest[0])),
                "note": opts.get("note", ""), "time": stamp}
        if opts.get("piece"):
            meta["piece"] = opts["piece"]
        if opts.get("choices"):
            meta["choices"] = [c.strip() for c in opts["choices"].split(",") if c.strip()]
        if opts.get("frames"):
            fw, fh = (int(v) for v in opts["frames"].lower().split("x"))
            meta["frames"] = {"w": fw, "h": fh, "ms": int(opts.get("ms", "133")),
                              "order": [int(v) for v in opts["order"].split(",")] if opts.get("order") else None}
        if opts.get("map") and opts.get("at"):
            x, y = (int(v) for v in opts["at"].split(","))
            fw, fh = (meta["frames"]["w"], meta["frames"]["h"]) if "frames" in meta else (16, 32)
            meta["ingame"] = "/api/ingame?" + urllib.parse.urlencode(
                {"map": opts["map"], "x": x, "y": y, "img": copies[0], "fw": fw, "fh": fh})
        json.dump(meta, open(os.path.join(SHOW, stamp + ".json"), "w"))
        if opts.get("open", "yes") != "no":
            open_page("/?ask=" + stamp)
        print("shown:", meta["title"], "(question %s)" % stamp)
        if opts.get("wait"):
            got = wait_answer(stamp, float(opts["wait"]) * 60)
            if not got:
                print("no answer within %s minutes (later: viewer.py answer %s)" % (opts["wait"], stamp))
                return 2
            print(json.dumps(got, indent=2))
    elif cmd == "answer":
        got = find_answer(rest[0])
        if not got:
            print("no answer yet to", rest[0])
            return 2
        print(json.dumps(got, indent=2))
    elif cmd == "board":
        start()
        title = urllib.parse.quote(opts.get("title", ""))
        open_page("/board?map=%s&title=%s" % (rest[0], title))
    elif cmd == "answers":
        files = sorted(f for f in os.listdir(ANSWERS) if f.endswith(".json"))
        for f in files[-int(opts.get("last", "5")):]:
            print("== " + f)
            print(open(os.path.join(ANSWERS, f)).read())
    elif cmd == "stop":
        if os.path.exists(PIDFILE):
            try:
                os.kill(int(open(PIDFILE).read()), signal.SIGTERM)
                print("viewer stopped")
            except (OSError, ValueError):
                print("viewer wasn't running")
        else:
            print("viewer wasn't running")
    else:
        print(__doc__)
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
