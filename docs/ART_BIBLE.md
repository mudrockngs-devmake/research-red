# Art bible: how Research Red looks

The visual rulebook. Every new sprite, picture and tile follows it, so a
character drawn this month matches one drawn next year, and art from the
Art Kit, a helper or a hand pass all looks like the same game. It's a
**draft** (2026-10-24), worked out from FireRed's own art and the cast's
reference designs. The owner has the final say: change anything here, and
the tools and the tracker follow.

How to *send* art (sizes, file names, folders) is in [ARTIST_GUIDE.md](ARTIST_GUIDE.md);
the tools are in [ART_TOOLS.md](ART_TOOLS.md); what's left to make is in
[ART_TRACKER.md](ART_TRACKER.md).

## 1. The feel
- **FireRed first.** New art should look like it shipped with FireRed / LeafGreen:
  the same pixel size, outlines, shading and proportions. If a new sprite stands
  next to a FireRed one and you can tell which is new, it isn't done.
- **Warm and cozy.** Research Red is a found-family road trip with scientists.
  Colours lean warm (tan skin, auburn and brown hair, soft whites); cool colours
  are for accents (Elm's teal, Ivy's navy). The interface is the field notebook
  (cream paper, ink).
- **Readable at a glance.** On a 240x160 screen, a character is recognised by
  their silhouette and 2-3 signature colours (Ivy: purple hair + white coat +
  coral; Elm: brown hair + open coat + teal). Those come first; details second.

## 2. Measured from FireRed (2026-10-24)
| | Overworld people (119 sheets) | Trainer battle pictures (34) |
|---|---|---|
| Frame | 16x32 | 64x64 |
| The figure itself | ~14 wide, ~20 tall (rows 11-30) | ~38 wide, ~60 tall |
| Colours used | ~12 (of 15 + see-through) | 15 (all of them) |
| Silhouette outline | black `#000000` (116 of 119) | black (33 of 34) |

## 3. Pixels and colours
- **One palette per sprite: 15 colours + colour 0 see-through.** Shared palettes
  (the players' `player.pal`, which the bike, surf and fishing sprites also use)
  are planned together.
- **Outline:** a black outline around the whole silhouette. Inside the figure,
  lines are the darkest shade of the colour they sit on (dark brown inside hair,
  not black).
- **Light from the top left.** Each material (hair, skin, coat, trousers) gets
  3 shades: light, middle, dark. Most of the area is the middle shade; the light
  shade is on top and left edges; the dark shade is underneath and on the right.
- **No blur or soft edges.** No anti-aliasing against the background, no
  gradients, no single stray pixels ("pillow shading" around edges is out too).
- **Snap to GBA colours** (5 bits per channel; the tools do this).
- **White isn't pure white:** coats use a light grey-lilac (`#eeeeff` / `#b4b4d5`,
  FireRed's own) so they don't glow.

## 4. Proportions
- **Overworld:** FireRed's chibi build: the head is about half the figure (hair
  rows ~11-16, face ~16-22, body ~23-27, legs ~28-30). Eyes are 1 pixel wide, 2 tall.
  Walking frames move the legs and bob the body by 1 pixel, like FireRed's.
- **Battle pictures:** about 3 heads tall (FireRed's trainers), a slightly big
  head and sturdy limbs: one-pixel-thin arms or legs disappear in-game. Standing
  three-quarter view facing left, feet near the bottom edge. Faces are only a
  few pixels: eyes, brows and mouth are placed by hand.
- **Back pictures:** upper body seen from behind, as large as FireRed's Red
  in his back picture; the throw follows Red's 5 frames.
- **Portraits:** head and chest, 64x64, the face in the upper middle.

## 5. Characters
Each character's signature colours and shapes live in their recipe in
`graphics/research_red/overworld/recipes.toml` (the `notes` line) and in their
reference picture. Rules for all of them:
- Outfits change only with the story (Ivy's swimsuit, Officer Jenny's uniform);
  each outfit is its own piece in the tracker.
- The professors' lab coats are open and white (lilac-grey shades); each
  professor's shirt colour is their accent.

## 6. Places and tiles
- Map pieces use FireRed's 16x16 metatiles and the tileset's existing palettes,
  so they need no new colours. If a piece needs a new colour, take a slot that
  tileset doesn't use.
- Furniture and buildings sit on the same top-down 3/4 view as FireRed's: you
  see the front and the top.
- Time of day is the engine's palette tint; don't draw separate night tiles.

## 7. How a piece gets finished
1. **Draft:** the Art Kit tools make a version that follows this page.
2. **Hand-finished:** in Aseprite (`aseprite.py open`), fix faces, hands and
   anything the tools can't judge; check it beside a FireRed sprite.
3. **Approved:** the owner looks at it in the viewer (in-game size and zoomed)
   and approves it; the tracker records it.
4. **In the game:** `art_gate.py apply`, then build, test, look.

## 8. For the owner to decide
- [ ] Outline: keep black like FireRed, or a very dark brown for a softer look?
- [ ] Battle pictures: FireRed proportions (now) or a little taller and more
      like the reference art?
- [ ] Any character whose signature colours should change from the reference art.
