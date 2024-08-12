//
// Created by alex on 8/12/24.
//

#include <defs.h>

int sq_on_board(const int sq) {
    return files_board[sq]==OFFBOARD ? 0 : 1;
}

int side_valid(const int side) {
    return (side==WHITE || side == BLACK) ? 1 : 0;
}

int file_rank_valid(const int fr) {
    return (fr >= 0 && fr <= 7) ? 1 : 0;
}

int piece_valid_empty(const int pce) {
    return (pce >= EMPTY && pce <= bK) ? 1 : 0;
}

int piece_valid(const int pce) {
    return (pce >= wP && pce <= bK) ? 1 : 0;
}
