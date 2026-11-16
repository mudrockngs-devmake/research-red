# Possible upgrades for Research Red (from claude-setup's away run, 2026-11-16)

The owner asked: "push some potential upgrades to Research Red … as we're gaining all this new
knowledge." These are **ideas to weigh, not approved work**. Each one says where the knowledge
came from (in `~/scripts/claude-setup/docs/`). This project is public, so anything that changes
GitHub settings or deletes a file waits for the owner.

## GitHub (from `research/github-upgrade-plan.md`)
1. **Protect `master`** (free on public repos): a ruleset that blocks force-pushes and deleting
   the branch. One `gh api` call; the commands are in the plan. Light.
2. **Dependabot for the Actions workflows** (`build`, `build-bps`, `smoke-tests`, `art-kit`):
   it opens a PR when an action used in a workflow has a newer version. Light.
3. **Build badge + social preview picture** for the README and link previews (the picture can
   only be uploaded on the website). Light; the owner picks the picture.
4. **Topics:** add `production` (the new studio labels). Whether the description should start
   with `[production]` is the owner's call: players see it.
5. **A stray file at the top level** called `as the Reporter) | game |#` (2.8 KB, a copy of the
   CREDITS text, probably from a shell mistake). Removing it is a delete: owner's OK first.
6. **Issues as the inbox:** the bug and idea forms already exist. The plan adds one shared
   `inbox` label across all projects so the hub can count new player reports in one place.

## Tools Claude now has (from `research/program-integrations.md`)
7. **Playtest recordings with chapters:** once the owner switches on OBS's remote control,
   Claude can start/stop OBS and drop markers ("Brock fight", "softlock here") that become
   YouTube chapters or bug timestamps. Useful for the playtest rounds. Medium.
8. **Phone ping when a long job ends:** a Discord webhook (`docs/accounts-to-set-up.md`) could
   say "build done / smoke tests failed" during away runs. Light, after the owner makes it.
9. **`apps`** shows which art/sound programs are installed and open; **`play`** starts a Steam
   game by name. Aseprite is installed through Steam (`E:\SteamLibrary`), so `aseprite.py`
   can rely on `Aseprite.exe -b` batch exports without opening the window.

## Bugs fixed today that touch this project
10. The hub's "template update waiting" count was wrong for about 1 version in 4 (Copier
    writes all-digit versions in quotes). Fixed in claude-setup; nothing to do here, but the
    count shown for this project is now right.

## Later (from `accounts-to-set-up.md`)
11. **PokéCommunity account** for a release thread when there's something to show (their
    rules: patches only, never ROM files, which our safety check already blocks).
12. **Hub "What needs me" screen** (planned) would list this project's open issues next to
    STATUS items, so player reports don't get missed.

## Later the same day: from the Pokémon decomp community research (`research/pokemon-decomp-community.md`)
Ideas only, to weigh with the owner. The research file has the reasons and sources.
13. **Try pokeemerald-expansion's FireRed mode in a small trial copy** (it builds FireRed since
    1.15.0, 2026-02-28): a modern battle engine, a test system and monthly fixes, but a big merge
    and some FRLG features are reported missing (Teachy TV, Help System). Decide only after the trial. Heavy.
14. **Automatic battle/script checks on every build** (expansion's `make check` idea, run headless
    in mGBA), so a playtest isn't the first place a bug shows up, even without switching bases. Medium-Heavy.
15. **Porytiles** for new tilesets drawn as PNG layers; **Poryscript** for new event scripts and
    dialogue (wraps text automatically). Medium each.
16. **Port single expansion features** if staying on pokefirered: debug menu, followers, DexNav-style
    search, modern move/ability data. Medium each.
17. **Pull pokefirered's upstream fixes now and then** (still updated, last 2026-10-20) and add a
    `modern` compiler build to the build check to catch warnings. Light.
18. **Say plainly when AI helped** in README/credits: the decomp scene turned against undisclosed AI
    work this year. Light.
