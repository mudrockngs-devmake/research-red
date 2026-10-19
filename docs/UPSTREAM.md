# The engine underneath (upstream)

Research Red is built on **pokefirered-expansion** (rh-hideout's expansion of
pret's FireRed disassembly). Their work keeps improving (new mechanics, fixes),
and every update we pull in can also break something of ours. So we update on
purpose, not by accident.

## Where we are
| | |
|---|---|
| Engine commit we started from | `582676ee8` "fix trainer approach" (2026-09-22) |
| Last engine update merged | none since the start |
| Our first commit | `91d94d219` (2026-10-15) |

## How our changes stay findable
- Our own files are prefixed `rr_` / `RR_` and live in `src/rr_*.c`, `include/rr_*.h`, `data/scripts/research_red/`, `graphics/research_red/`, `tools/research_red/`, `data/research_red/`.
- Every edit inside an engine file has a `Research Red` comment: `grep -rn "Research Red" src include data` finds them all. These are the places a merge can conflict.

## When to update
Once per milestone (v0.1, v0.2...), right after a playtest round, never in the middle of one. Or sooner if a fix or feature we need lands upstream.

## How to update
1. Add the remote once: `git remote add upstream <pokefirered-expansion's GitHub URL>`. Confirm the exact repo first; this fork's recent engine commits are by its maintainer cawtds, and the expansion family lives under rh-hideout.
2. `git fetch upstream`, read their changelog/releases since our commit for anything that touches what we changed (battle setup, start menu, trainer slides, field message box, save blocks, intro).
3. On a branch: `git checkout -b engine-update && git merge upstream/<their release tag or master>`.
4. Fix conflicts in the `Research Red`-tagged spots. Check `include/global.h` save blocks especially (see [SAVES.md](SAVES.md)).
5. Build, `rr_check.py`, smoke tests, a quick manual run of every scene.
6. Merge into master, update the table above, and give it its own test build so the owner plays it.
