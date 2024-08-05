//
// Created by alex on 8/5/24.
//

#include <stdio.h>

#include "defs.h"

U64 generate_position_key(const S_BOARD *pos) {
    int sq = 0;
    U64 final_key = 0;
    int piece = EMPTY;

    //loop through the board
    for (sq = 0; sq < BOARD_SQ_NUMBER; sq++) {
        piece = pos->pieces[sq]; //set piece to the value of the square
        if (piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) { //if valid position and not empty square
            ASSERT(piece >= wP && piece <= bK);
            final_key ^= piece_keys[piece][sq];
        }
    }
    if (pos->side == WHITE) {
        final_key ^= side_key;
    }
    if (pos->en_passant != NO_SQ) {
        ASSERT(pos->en_passant >= 0 && pos->en_passant < BOARD_SQ_NUMBER);
        final_key ^= piece_keys[EMPTY][pos->en_passant];
    }

    ASSERT(pos->castle_permission >= 0 && pos->castle_permission <= 15);//remember castle permission is a 4bit number like 1111 = 15
    final_key ^= castle_key[pos->castle_permission];

    return final_key;
}