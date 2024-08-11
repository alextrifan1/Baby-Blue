//
// Created by alex on 8/11/24.
//

 #include "../include/defs.h"

// use this to calculate movement
const int knight_direction[8] = {-8, -19, -21, -12, 8, 19, 21, 12 };
const int rook_direction[4] = {-1, -10, 1, 10};
const int bishop_direction[4] = {-9, -11, 11, 9};
const int king_direction[8] = {-1, -10, 1, 10, -9, -11, 11, 9};

/// check if a square is attacked by a piece
/// @param sq the square we check
/// @param side what side is attacking
/// @param pos
/// @return
int square_attacked(const int sq, const int side, const S_BOARD *pos) {
    int piece, index, temp_square, direction;

    // pawns
    if (side == WHITE) {
        if (pos->pieces[sq - 11] == wP || pos->pieces[sq - 9] == wP) {
            return TRUE;
        }
    } else { // side == BLACK
        if (pos->pieces[sq + 11] == bP || pos->pieces[sq+9] == bP) {
            return TRUE;
        }
    }

    // knights
    for (index = 0; index < 8; index++) {
        piece = pos->pieces[sq + knight_direction[index]];
        if (piece != OFFBOARD && piece != EMPTY) {
            if (IsKn(piece) && piece_color[piece] == side) {
                // is the piece a knight of right color
                return TRUE;
            }
        }
    }

    // rooks, queens
    for (index = 0; index < 4; index++) {
        direction = rook_direction[index];
        temp_square = sq + direction;
        piece = pos->pieces[temp_square];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (IsRQ(piece) && piece_color[piece] == side) {
                    return TRUE;
                }
                break;
            }
            temp_square += direction;
            piece = pos->pieces[temp_square];
        }
    }

    // bishop, queens
    for (index = 0; index < 4; index++) {
        direction = bishop_direction[index];
        temp_square = sq + direction;
        piece = pos->pieces[temp_square];
        while (piece != OFFBOARD) {
            if (piece != EMPTY) {
                if (IsBQ(piece) && piece_color[piece] == side) {
                    return TRUE;
                }
                break;
            }
            temp_square += direction;
            piece = pos->pieces[temp_square];
        }
    }

    // kings
    for (index = 0; index < 8; index++) {
        piece = pos->pieces[sq + king_direction[index]];
        if (piece != OFFBOARD && piece != EMPTY) {
            if (IsKi(piece) && piece_color[piece] == side) {
                return TRUE;
            }
        }
    }
    return FALSE;
}