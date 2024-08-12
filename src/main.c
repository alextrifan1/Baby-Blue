//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/defs.h"

#define PAWNMOVES "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"

int main() {
     all_init();

     S_BOARD board[1];

     parse_fen(PAWNMOVES, board);
     print_board(board);

     S_MOVELIST list[1];

     generate_all_moves(board, list);

     print_movelist(list);

     return 0;
}
