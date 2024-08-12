//
// Created by alex on 8/12/24.
//
#include <stdio.h>
#include <defs.h>

#define MOVE(from,to,captured,promoted,flag) ( (from) | ((to) << 7) | ( (captured) << 14 ) | ( (promoted) << 20 ) | (flag))
#define SQOFFBOARD(sq) (files_board[(sq)]==OFFBOARD) // don't want to do a function call rn

/// adds a quiet move to the moves list
/// @param pos board structure
/// @param move board structure
/// @param list move list
void add_quiet_move(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

/// adds a capture move to the moves list
/// @param pos board structure
/// @param move board structure
/// @param list
void add_capture_move(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

/// adds an en passant move to the moves list
/// @param pos board structure
/// @param move board structure
/// @param list move list
void add_enpassante_move(const S_BOARD *pos, int move, S_MOVELIST *list) {
    list->moves[list->count].move = move;
    list->moves[list->count].score = 0;
    list->count++;
}

/// deals with the case of a pawn capturing a piece (and promoting to a new piece)
void add_white_pawn_capture_move(const S_BOARD *pos, const int from, const int to, const int captured, S_MOVELIST *list) {
    if (ranks_board[from] == RANK_7) {
        add_capture_move(pos, MOVE(from, to, captured, wQ, 0), list);
        add_capture_move(pos, MOVE(from, to, captured, wR, 0), list);
        add_capture_move(pos, MOVE(from, to, captured, wB, 0), list);
        add_capture_move(pos, MOVE(from, to, captured, wN, 0), list);
    } else {
        add_capture_move(pos, MOVE(from, to, captured, EMPTY, 0), list);
    }
}

/// deals with the case of a pawn advancing a position without capturing a piece (and promoting to a new piece)
void add_white_pawn_move(const S_BOARD *pos, const int from, const int to, S_MOVELIST *list) {
    if (ranks_board[from] == RANK_7) {
        add_capture_move(pos, MOVE(from, to, EMPTY, wQ, 0), list);
        add_capture_move(pos, MOVE(from, to, EMPTY, wR, 0), list);
        add_capture_move(pos, MOVE(from, to, EMPTY, wB, 0), list);
        add_capture_move(pos, MOVE(from, to, EMPTY, wN, 0), list);
    } else {
        add_capture_move(pos, MOVE(from, to, EMPTY, EMPTY, 0), list);
    }
}

void generate_all_moves(const S_BOARD *pos, S_MOVELIST *list) {
    ASSERT(check_board(pos));

    list->count = 0;

    int piece = EMPTY;
    int side = pos->side;
    int sq = 0, temp_sq = 0, piece_number = 0;

    if (side == WHITE) {
        for (piece_number = 0; piece_number < pos->pieces_number[wP]; piece_number++) { // loop through all wP
            sq = pos->p_list[wP][piece_number]; // get the square of the piece
            ASSERT(sq_on_board(sq));

            if (pos->pieces[sq+10] == EMPTY) { // advancing a position
                add_white_pawn_move(pos, sq, sq+10, list);
                if (ranks_board[sq] == RANK_2 && pos->pieces_number[sq+20] == EMPTY) { // white pawns can advance two squares if on rank 2
                    add_quiet_move(pos, MOVE(sq, (sq + 20), EMPTY, EMPTY, MFLAGPS), list);
                }
            }

            // capturing
            if (!SQOFFBOARD(sq + 9) && piece_color[pos->pieces[sq+9]] == BLACK) {
                add_white_pawn_capture_move(pos, sq, sq+9, pos->pieces[sq+9], list);
            }
            if (!SQOFFBOARD(sq + 11) && piece_color[pos->pieces[sq+11]] == BLACK) {
                add_white_pawn_capture_move(pos, sq, sq+11, pos->pieces[sq+11], list);
            }

            // capturing with en passant
            if (pos->en_passant != NO_SQ) {
                if (sq + 9 == pos->en_passant) {
                    add_capture_move(pos, MOVE(sq, sq+9, EMPTY, EMPTY, MFLAGEP), list);
                }
                if (sq + 11 == pos->en_passant) {
                    add_capture_move(pos, MOVE(sq, sq+11, EMPTY, EMPTY, MFLAGEP), list);
                }
            }
        }
    } else { // side is BLACK

    }

}