"""Research Red: names -> addresses for the test runner (rr_run).

rr_run only understands raw addresses. This module reads the built game to
turn names into them, so smoke.py can say "story step is 3" or "we're on
Route 1" instead of "*0x03004F70+0x3062:2 == 3":
  - RAM symbols (gSaveBlock1Ptr, gMain...) come from pokefirered.elf (nm)
  - struct offsets and constants (FLAG_X, MAP_X, SPECIES_X) are worked out by
    compiling a tiny C file against the game's headers, so they stay right
    when the structs or constants change.
Works for any pokefirered / pokeemerald-style decomp; nothing here is
specific to Research Red's story.

Each condition method returns a string for rr_run's check / until / idle
commands; several can be joined with " && ".
"""
import os
import re
import subprocess
import tempfile

HEADERS = ["global.h", "main.h", "constants/flags.h", "constants/vars.h",
           "constants/map_groups.h", "constants/species.h"]
OFFSETS = {  # name -> C expression
    "sb1_location": "offsetof(struct SaveBlock1, location)",
    "sb1_flags": "offsetof(struct SaveBlock1, flags)",
    "sb1_vars": "offsetof(struct SaveBlock1, vars)",
    "main_callback2": "offsetof(struct Main, callback2)",
    "vars_start": "VARS_START",
    "script_shutdown": "2",  # CONTEXT_SHUTDOWN in src/script.c
}


class GameMemory:
    def __init__(self, repo, elf, game_define="FIRERED"):
        self.repo = repo
        self.define = game_define
        out = subprocess.run(["arm-none-eabi-nm", elf], capture_output=True, text=True, check=True).stdout
        self.symbols = {}
        for line in out.splitlines():
            parts = line.split()
            if len(parts) == 3:
                self.symbols[parts[2]] = int(parts[0], 16)
        self._consts = {}
        self._resolve(OFFSETS)

    # --- lookups ----------------------------------------------------------
    def _resolve(self, exprs):
        """Compile name -> C expression pairs and cache their integer values."""
        names = list(exprs)
        src = "".join(f'#include "{h}"\n' for h in HEADERS)
        src += "const int rr_values[] = {\n" + "".join(f"    (int)({exprs[n]}),\n" for n in names) + "};\n"
        with tempfile.TemporaryDirectory() as tmp:
            c = os.path.join(tmp, "rr_values.c")
            open(c, "w").write(src)
            r = subprocess.run(["arm-none-eabi-gcc", "-iquote", os.path.join(self.repo, "include"),
                                "-DMODERN=1", "-DTESTING=0", f"-D{self.define}", "-std=gnu17",
                                "-mthumb", "-mabi=apcs-gnu", "-march=armv4t", "-S", "-o", "-", c],
                               capture_output=True, text=True)
        if r.returncode:
            raise ValueError(f"can't resolve {', '.join(names)}:\n{r.stderr[-800:]}")
        body = r.stdout.split("rr_values:", 1)[1]
        values = [int(v) for v in re.findall(r"\.word\s+(-?\d+)", body)][:len(names)]
        self._consts.update(zip(names, values))

    def const(self, name):
        """Value of a game constant or expression, e.g. FLAG_SYS_POKEDEX_GET."""
        if name not in self._consts:
            self._resolve({name: name})
        return self._consts[name]

    def sym(self, name):
        if name not in self.symbols:
            raise KeyError(f"symbol {name} not in the elf")
        return self.symbols[name]

    def _sb1(self, offset, size):
        return f"*{self.sym('gSaveBlock1Ptr'):#x}+{offset:#x}:{size}"

    # --- addresses (for peek) --------------------------------------------
    def var_addr(self, var):
        vid = self.const(var) if isinstance(var, str) else var
        return self._sb1(self.const("sb1_vars") + (vid - self.const("vars_start")) * 2, 2)

    def flag_addr(self, flag):
        fid = self.const(flag) if isinstance(flag, str) else flag
        return self._sb1(self.const("sb1_flags") + fid // 8, 1), 1 << (fid % 8)

    def map_addr(self):
        """mapGroup then mapNum as one 16-bit read: group | num << 8."""
        return self._sb1(self.const("sb1_location"), 2)

    def bag_slot_addr(self, pocket, index):
        """itemId of slot `index` in a Bag pocket (e.g. "TMsHMs"), 2 bytes."""
        return self._sb1(self.const(f"offsetof(struct SaveBlock1, bag.{pocket}[{index}].itemId)"), 2)

    def player_xy_addrs(self):
        """The player's map x and y (2 bytes each, plain map coordinates as in
        route_plan.py). For peek, e.g. to find where a scene left you."""
        base = f"{self.sym('gSaveBlock1Ptr'):#x}"
        off = self.const("offsetof(struct SaveBlock1, pos)")
        return f"*{base}+{off:#x}:2", f"*{base}+{off + 2:#x}:2"

    def player_at(self, x, y):
        """True while the player stands on map tile (x, y), read live from the
        player's object (the first object; its coordinates carry the 7-tile
        map border offset)."""
        base = self.sym("gObjectEvents") + self.const("offsetof(struct ObjectEvent, currentCoords)")
        return f"{base:#x}:2 == {x + 7} && {base + 2:#x}:2 == {y + 7}"

    def party_addr(self):
        return f"{self.sym('gPlayerPartyCount'):#x}:1"

    def text_bytes(self, text):
        """A string in the game's character encoding (charmap.txt), plus EOS."""
        if not hasattr(self, "_charmap"):
            self._charmap = {}
            for line in open(os.path.join(self.repo, "charmap.txt"), encoding="utf-8"):
                m = re.match(r"'(.)'\s*=\s*([0-9A-Fa-f]{2})\s*$", line.strip())
                if m:
                    self._charmap.setdefault(m.group(1), int(m.group(2), 16))
        return [self._charmap[c] for c in text] + [0xFF]

    def party_nickname_is(self, slot, text):
        """The nickname of party slot `slot` is exactly `text`."""
        base = self.sym("gPlayerParty") + slot * self.const("sizeof(struct Pokemon)") \
            + self.const("offsetof(struct BoxPokemon, nickname)")
        return " && ".join(f"{base + i:#x}:1 == {b:#x}" for i, b in enumerate(self.text_bytes(text)))

    def map_value(self, map_name):
        m = self.const(map_name)  # MAP_X = num | group << 8
        return (m >> 8) | ((m & 0xFF) << 8)

    # --- conditions ------------------------------------------------------
    def var_is(self, var, value, op="=="):
        return f"{self.var_addr(var)} {op} {value}"

    def flag_set(self, flag):
        addr, bit = self.flag_addr(flag)
        return f"{addr} & {bit}"

    def flag_clear(self, flag):
        addr, bit = self.flag_addr(flag)
        return f"{addr} !& {bit}"

    def bag_slot_filled(self, pocket, index):
        return f"{self.bag_slot_addr(pocket, index)} != 0"

    def on_map(self, map_name):
        return f"{self.map_addr()} == {self.map_value(map_name)}"

    def party_count(self, value, op="=="):
        return f"{self.party_addr()} {op} {value}"

    def overworld(self):
        """The main loop is the overworld (not a battle, menu or transition)."""
        cb2 = self.sym("gMain") + self.const("main_callback2")
        return f"{cb2:#x}:4 == {self.sym('CB2_Overworld') | 1:#x}"

    def in_battle(self):
        cb2 = self.sym("gMain") + self.const("main_callback2")
        return f"{cb2:#x}:4 == {self.sym('BattleMainCB2') | 1:#x}"

    def textbox_closed(self):
        return f"{self.sym('sMessageBoxType'):#x}:1 == 0"

    def field_free(self):
        """On the map, no script running, no text box: the player can walk."""
        return " && ".join([
            self.overworld(),
            f"{self.sym('sGlobalScriptContextStatus'):#x}:1 == {self.const('script_shutdown')}",
            f"{self.sym('sLockFieldControls'):#x}:1 == 0",
            self.textbox_closed(),
        ])
