---
name: new-scene
description: Build or rework a Research Red story scene or cutscene (script, positions, dialogue, battles, flags) following the project's conventions, with a smoke test. Use when adding or changing a cutscene, a scripted battle, an NPC conversation, or anything in data/scripts/research_red/.
---

# New or reworked scene

Conventions: docs/DEV_GUIDE.md, docs/STYLE_GUIDE.md, docs/DESIGN.md (rules the scene must follow).

1. **Spec first**: the owner's scene spec in `docs/scenes/` wins over the animatic (`docs/animatic/`).
   No spec? Write one with the template in docs/CHAT_TO_CODE.md and get the owner's OK on the beats.
   Positions are a "how it looks" question: stage them on the viewer's scene board
   (`tools/research_red/viewer/viewer.py board MAP_X`).
2. **Script**: one file `data/scripts/research_red/rr_<scene>.inc`; the FireRed map keeps only its hook.
   Ivy: `rr_ivy_to_scene LOCALID_X_IVY` at the start, `rr_ivy_follow LOCALID_X_IVY` at the end (the scene's
   Ivy object needs a hide flag). `RR_` prefix on labels; a `Research Red` comment on any engine-file edit.
3. **Text**: proper case, 34 characters a line, `NPC_TEXT_COLOR_NEUTRAL`, speakers as `Name: ` (portraits),
   each character's voice (docs/characters/). Draft lines are marked as drafts for the owner.
4. **Battles**: levels and teams follow DESIGN.md (caps, the lab plan, rematch tiers). Watched fights are
   scripted cutscenes, not AI battles (#105).
5. **Flags and vars**: register every new one in docs/FLAGS_VARS.md; story step `VAR_0x40FE` per CLAUDE.md.
6. **Test**: a smoke scenario in `tools/research_red/test/smoke.py` that plays the scene to its end and checks
   memory (`expect(...)`), plus a debug skip if the scene is hard to reach. Run it, open its contact sheet.
7. **Exports**: `python3 tools/research_red/export_story.py --sync` (docs/story/), `rr_check.py`.
8. Commit with the issue number; tick docs/CHECKLIST.md if the scene is on it.
