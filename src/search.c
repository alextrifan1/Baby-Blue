//
// Created by alex on 9/19/24.
//

#include <stdio.h>
#include "defs.h"

static void checkup() {
    // check if time up or stop from gui
}

static int is_repetition(const S_BOARD *pos) {
    int index;

    for (index = pos->history_ply - pos->fifty_moves; index < pos->history_ply; index++) {

        ASSERT(index >= 0 && index < MAX_GAME_MOVES);

        if (pos->position_key == pos->history[index].positon_key) {
            return TRUE;
        }
    }
    return FALSE;
}

static void clear_for_search(S_BOARD *pos, S_SEARCHINFO *info) {

}

static int quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {
    return 0;
}

static int alpha_beta(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info, int do_null) {
    return 0;
}


void search_position(S_BOARD *pos, S_SEARCHINFO *info) {
    // iterative deepening, search init
}