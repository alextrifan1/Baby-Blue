//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/defs.h"

#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"


int main() {
     all_init();


     S_BOARD board[1];

     parse_fen(FEN2, board);
     parse_fen(FEN3, board);
     print_board(board);

     ASSERT(check_board(board));

     return 0;
}
