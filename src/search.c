//
// Created by alex on 9/19/24.
//

#include <stdio.h>
#include "defs.h"

int is_repetition(const S_BOARD *pos) {
    int index;

    for (index = pos->history_ply - pos->fifty_moves; index < pos->history_ply; index++) {
        if (pos->position_key == pos->history[index].positon_key) {
            return TRUE;
        }
    }
    return FALSE;
}

void search_position(S_BOARD pos) {

}