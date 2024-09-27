//
// Created by alex on 9/26/24.
//

#include <stdio.h>
#include <defs.h>

const int pawn_table[64] = {
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0	,
    10	,	10	,	0	,	-10	,	-10	,	0	,	10	,	10	,
    5	,	0	,	0	,	5	,	5	,	0	,	0	,	5	,
    0	,	0	,	10	,	20	,	20	,	10	,	0	,	0	,
    5	,	5	,	5	,	10	,	10	,	5	,	5	,	5	,
    10	,	10	,	10	,	20	,	20	,	10	,	10	,	10	,
    20	,	20	,	20	,	30	,	30	,	20	,	20	,	20	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int knight_table[64] = {
    0	,	-10	,	0	,	0	,	0	,	0	,	-10	,	0	,
    0	,	0	,	0	,	5	,	5	,	0	,	0	,	0	,
    0	,	0	,	10	,	10	,	10	,	10	,	0	,	0	,
    0	,	0	,	10	,	20	,	20	,	10	,	5	,	0	,
    5	,	10	,	15	,	20	,	20	,	15	,	10	,	5	,
    5	,	10	,	10	,	20	,	20	,	10	,	10	,	5	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int bishop_table[64] = {
    0	,	0	,	-10	,	0	,	0	,	-10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	10	,	15	,	20	,	20	,	15	,	10	,	0	,
    0	,	0	,	10	,	15	,	15	,	10	,	0	,	0	,
    0	,	0	,	0	,	10	,	10	,	0	,	0	,	0	,
    0	,	0	,	0	,	0	,	0	,	0	,	0	,	0
};

const int rook_table[64] = {
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0	,
    25	,	25	,	25	,	25	,	25	,	25	,	25	,	25	,
    0	,	0	,	5	,	10	,	10	,	5	,	0	,	0
};

const int mirror64[64] = {
    56, 57, 58, 59, 60, 61, 62, 63,
    48, 49, 50, 51, 52, 53, 54, 55,
    40, 41, 42, 43, 44, 45, 46, 47,
    32, 33, 34, 35, 36, 37, 38, 39,
    24, 25, 26, 27, 28, 29, 30, 31,
    16, 17, 18, 19, 20, 21, 22, 23,
    8,  9,  10, 11, 12, 13, 14, 15,
    0,  1,  2,  3,  4,  5,  6,  7
};

#define MIRROR64(sq) (mirror64[(sq)])

int evaluate_position(const S_BOARD *pos) {
    int piece;
    int piece_number;
    int sq;
    int score = pos->material[WHITE] - pos->material[BLACK];

    piece = wP;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; piece_number++) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score += pawn_table[SQ64(sq)];
    }

    piece = bP;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; piece_number++) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score -= pawn_table[MIRROR64(SQ64(sq))];
    }

    piece = wN;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score += knight_table[SQ64(sq)];
    }

    piece = bN;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score -= knight_table[MIRROR64(SQ64(sq))];
    }

    piece = wB;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score += bishop_table[SQ64(sq)];
    }
    
    piece = bB;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score -= bishop_table[MIRROR64(SQ64(sq))];
    }

    piece = wR;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score += rook_table[SQ64(sq)];
    }

    piece = bR;
    for (piece_number = 0; piece_number < pos->pieces_number[piece]; ++piece_number) {
        sq = pos->p_list[piece][piece_number];
        ASSERT(sq_on_board(sq));
        score -= rook_table[MIRROR64(SQ64(sq))];
    }

    if (pos->side == WHITE) {
        return score;
    } else {
        return -score;
    }
}

