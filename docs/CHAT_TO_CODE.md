# How chat design becomes code

1. The owner designs scenes, dialogue, teams and systems in the claude.ai Research Red project.
2. Each finished design is saved as a scene spec in `docs/scenes/` using the template below (the owner pastes it in or asks Claude Code to create it).
3. Claude Code implements the spec, ticks `docs/CHECKLIST.md`, and reports engine limits back to the owner.

## Scene spec template
```
# Scene: <name>            Milestone: v0.x
Map(s): <map names>        Trigger: <when it plays, which var/flag>
Cast and start positions: <who, where, facing>
Beats:
1. <action or line>  (Speaker: line, max 34 chars per line)
2. ...
Battles: <trainer, team, levels, doubles?, lines every 2 faints, last-Pokemon line>
Flags/vars set at the end: <...>
Notes / placeholders: <...>
```
