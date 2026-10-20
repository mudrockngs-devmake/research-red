#ifndef GUARD_RR_REMATCH_H
#define GUARD_RR_REMATCH_H

// Research Red: rematches that grow with your badges (src/rr_rematch.c)
u32 RR_GetRematchTier(void);
bool32 RR_IsRematchTrainer(u16 trainerId);
void RR_AdjustRematchParty(struct Pokemon *party, u16 trainerId, u32 count);

#endif // GUARD_RR_REMATCH_H
