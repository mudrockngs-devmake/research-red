# Wild Pokemon, one file per area

Edit a file, then run `python3 tools/research_red/build_encounters.py` and rebuild.

```
map: MAP_ROUTE1          # which map this file is for
[grass]                  # sections: grass, surf, rock smash, old rod, good rod, super rod
rate 21                  # optional: how often encounters happen (FireRed's usual if left out)
Pidgey      2-4   20     # name, levels, percent
[grass night]            # optional night version of any section
```

- Each section's percents add up to 100.
- The game has fixed slots (grass: 20/20/10/10/10/10/5/5/4/4/1/1, surf: 60/30/5/4/1,
  old rod: 70/30, good rod: 60/20/20, super rod: 40/40/15/4/1). Percents are built from
  those, so 20, 15 (10+5), 9 (5+4), 2 (1+1) work; the tool warns if one can't be made.
- If an area has no night section, the day list is used all day.
- Night is the game's clock (the same one that tints the screen at night).
- The Field Guide (Start menu) shows the real list for where you're standing.
