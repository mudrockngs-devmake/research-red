#ifndef GUARD_RR_SAVE_VERSION_H
#define GUARD_RR_SAVE_VERSION_H

// Research Red: save layout version. Bump it (and add a line to the table in
// docs/SAVES.md) whenever a change makes older saves unsafe to keep playing:
// anything that changes the size or order of the save blocks, or reuses a
// flag/var an old save might have set. See docs/SAVES.md.
#define RR_SAVE_VERSION 1

void RR_CheckSaveVersion(void);
bool32 RR_TryStartSaveVersionWarning(void);

#endif // GUARD_RR_SAVE_VERSION_H
