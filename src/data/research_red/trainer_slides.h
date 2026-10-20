// Research Red: trainer slide lines (included inside sTrainerSlides in
// src/trainer_slide.c). Placeholder dialogue from the animatic; rewrite freely.
//   BEFORE_FIRST_TURN   when the battle starts
//   RR_TWO_DOWN         2 of their Pokemon have fainted (once per battle)
//   RR_FOUR_DOWN        4 have fainted
//   LAST_SWITCHIN       that trainer sends out their last Pokemon
// In Elm & Jenny's doubles each of them has their own last line.

        [TRAINER_RR_ELM_R22_BULBASAUR] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_BULBASAUR] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_BULBASAUR] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_BULBASAUR] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_R22_CHARMANDER] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_CHARMANDER] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_CHARMANDER] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_CHARMANDER] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_R22_SQUIRTLE] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_SQUIRTLE] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_SQUIRTLE] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_SQUIRTLE] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_R22_CHIKORITA] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_CHIKORITA] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_CHIKORITA] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_CHIKORITA] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_R22_CYNDAQUIL] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_CYNDAQUIL] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_CYNDAQUIL] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_CYNDAQUIL] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_R22_TOTODILE] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: Rematch! This time I read a book!\pJenny: He read the back of a book.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_R22_TOTODILE] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! Okay. That was the warm-up.\pJenny: That was not the warm-up, dear.\p{B_PLAYER_NAME}: It looked like a very thorough warm-up from here.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, we are losing.\pElm: Strategically losing!\pIvy: That's the best kind, Elm.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Legends. And dinner plans.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_ELM_PEWTER_TOTODILE] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Elm: I've been training since breakfast! Second breakfast!\pJenny: He means lunch.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Elm: Last one! This is where legends are made!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_RR_JENNY_PEWTER_TOTODILE] =
        {
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Elm: Okay! That was the warm-up!\pJenny: Dear, we just said this battle is the warm-up.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Jenny: In my professional opinion, they are very good.\pIvy: In my personal opinion, so are you.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Jenny: Still legends. Still dinner.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_LEADER_BROCK] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Brock: My rock-hard willpower is evident even in my Pokémon! Let's go!{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_TWO_DOWN] = COMPOUND_STRING("Brock: Solid teamwork. You cover each other.\pIvy: We have practice. He covers me in conversations.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_RR_FOUR_DOWN] = COMPOUND_STRING("Brock: Four down? You two are the real deal.\p{B_PLAYER_NAME}: She's the real deal. I'm the snacks.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Brock: Onix! This is our moment. Steady, like we trained.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_CAMPER_LIAM] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Liam: You'll never reach Brock! He's busy making lunch!{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Liam: Diglett, dig deep!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_BUG_CATCHER_RICK] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Rick: Two grown-ups! With a net! No wait, that's a clipboard.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_BUG_CATCHER_DOUG] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Doug: My Kakuna is evolving any second now. Any second.{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Doug: Any second now!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_BUG_CATCHER_ANTHONY] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Anthony: I have two Caterpie. They're twins. Their names are Cater and Pie.{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_BUG_CATCHER_CHARLIE] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Charlie: Metapod knows Harden! It's the best move! It's the only move!{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Charlie: Harden! Harder!{PAUSE_UNTIL_PRESS}"),
        },

        [TRAINER_BUG_CATCHER_SAMMY] =
        {
            [TRAINER_SLIDE_BEFORE_FIRST_TURN] = COMPOUND_STRING("Sammy: I caught a Pikachu in here! Everybody says it's lucky!{PAUSE_UNTIL_PRESS}"),
            [TRAINER_SLIDE_LAST_SWITCHIN] = COMPOUND_STRING("Sammy: Come on, lucky Pikachu!{PAUSE_UNTIL_PRESS}"),
        },
