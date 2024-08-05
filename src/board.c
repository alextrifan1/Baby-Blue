//
// Created by alex on 8/5/24.
//

#include <stdio.h>
#include "defs.h"

/// the function should generate an empty board, all pieces, moves set to zero
/// @param pos our board structure
void reset_board(S_BOARD *pos) {
    int index;

    for (index = 0; index < BOARD_SQ_NUMBER; index++) {
        pos->pieces[index] = OFFBOARD;
    }
    for (index = 0; index < 64; index++) {
        pos->pieces[SQ120(index)] = EMPTY;
    }
    for (index = 0; index < 3; index++) {
        pos->big_pieces[index] = 0;
        pos->major_pieces[index] = 0;
        pos->minor_pieces[index] = 0;
        pos->pawns[index] = 0ULL;
    }

    for (index = 0; index < 13; index++) {
        pos->pieces_number[index] = 0;
    }

    pos->king_square[WHITE] = pos->king_square[BLACK] = NO_SQ;

    pos->side = BOTH; //for handling errors later
    pos->en_passant = NO_SQ;
    pos->fifty_moves = 0;

    pos->ply = 0;
    pos->history_ply = 0;

    pos->castle_permission = 0;
    pos->positon_key = 0ULL;
}
