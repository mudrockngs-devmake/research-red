---
name: owner-notes
description: Turn the owner's notes into issues and decisions without misreading them. Use when the owner sends notes, ideas, playtest notes, feedback, a design ramble or a voice-typed message with several points (long, run-on, mixing decisions and side thoughts), or says "notes", "my notes", "here's what I think", "write this down", "file these".
---

# Owner notes: read back first, then file

The owner often voice-types. Long notes mix decisions, ideas, bugs and thinking out
loud, and names come through garbled. Filing them straight away risks writing down
something they didn't mean. So: read back, get an OK, then file.

A single clear instruction ("fix the typo in Oak's line") is not notes: just do it.

1. **Fix the names** against `docs/GLOSSARY.md` (Elmo = Elm, batch = badge, blame = Blaine...).
   A new mix-up: add it to the glossary's "heard as" column.
2. **Read back, numbered**, in plain words, sorted into:
   - **Decisions** (how something should work): note which `docs/DESIGN.md` section each one changes.
   - **Ideas** (maybe later)
   - **Bugs** (something wrong in the game)
   - **Questions back** (only where the notes really are unclear: say how you read it and ask)
   For each, say where it would go: a new issue (labels), a comment on an existing issue (#),
   DESIGN.md, the story bible, or "just noted". Check open issues first (`gh issue list`,
   search) so nothing gets filed twice.
3. **Ask for the OK in one line**: "All fine, or tell me the numbers to change." Ask
   nothing else; the owner prefers answering a batch at once.
4. **On the OK, file it**: issues (labels, milestone if obvious), comments, DESIGN.md
   updates with the date and issue number. Commit and push the doc changes.
5. **Report back in a few lines**: what went where, with issue numbers.

Keep private things private: personal details, inspirations, anything the owner marks private
goes into Claude's memory only, never into an issue, doc or commit (the de-personalized
version can go public).
