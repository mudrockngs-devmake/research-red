#ifndef GUARD_CONFIG_DEBUG_H
#define GUARD_CONFIG_DEBUG_H

// Research Red: debug tools are ON for test builds.
// Set these back to DISABLED_ON_RELEASE before any public release.

// Overworld Debug
#define DEBUG_OVERWORLD_MENU            TRUE                // Enables an overworld debug menu to change flags, variables, giving pokemon and more.
#define DEBUG_OVERWORLD_HELD_KEYS       (B_BUTTON)          // The keys required to be held to open the debug menu.
#define DEBUG_OVERWORLD_TRIGGER_EVENT   pressedStartButton  // The event that opens the menu when holding the key(s) defined in DEBUG_OVERWORLD_HELD_KEYS.
#define DEBUG_OVERWORLD_IN_MENU         TRUE                // Research Red: adds a "Debug" entry to the Start menu (easier on a phone than B + Start).

// Battle Debug Menu
#define DEBUG_BATTLE_MENU               FALSE   // Research Red: off (#82): SELECT in battle opened it by accident (a dark list of Moves/Ability/Held Item...). Set TRUE to debug a battle.
#define DEBUG_AI_DELAY_TIMER            FALSE   // If set to TRUE, displays the number of frames it takes for the AI to choose a move.

// Pokémon Debug
#define DEBUG_POKEMON_SPRITE_VISUALIZER DISABLED_ON_RELEASE // Enables a debug menu for Pokémon sprites and icons, accessed by pressing Select in the summary screen.

#endif // GUARD_CONFIG_DEBUG_H
