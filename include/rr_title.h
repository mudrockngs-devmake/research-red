#ifndef GUARD_RR_TITLE_H
#define GUARD_RR_TITLE_H

// Research Red (#12): creator splash, title screen, new-game notices (src/rr_title.c)
bool32 RR_StartSplashOnce(MainCallback next); // FALSE if it already played since power-on
void CB2_RR_InitTitleScreen(void);
void RR_StartNewGameNotice(void);              // then the intro call

#endif // GUARD_RR_TITLE_H
