//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/defs.h"

#define FEN0 "8/3q1p2/8/5P2/4Q3/8/8/8 w - - 0 2"
#define FEN1 "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
#define FEN2 "rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq c6 0 2"
#define FEN3 "rnbqkbnr/pp1ppppp/8/2p5/4P3/5N2/PPPP1PPP/RNBQKB1R b KQkq - 1 2"

void show_sq_attacked_by_side(const int side, const S_BOARD *pos) {
     int rank = 0;
     int file = 0;
     int sq = 0;

     printf("\n\nSquares attacked by:%c\n", side_char[side]);
     for (rank = RANK_8; rank >= RANK_1; rank--) {
          for (file = FILE_A; file <= FILE_H; file++) {
               sq = FR2SQ(file, rank);
               if (square_attacked(sq, side, pos) == TRUE) {
                    printf("X");
               } else {
                    printf("-");
               }
          }
          printf("\n");
     }
     printf("\n\n");
}


int main() {
     all_init();


     S_BOARD board[1];

     parse_fen(FEN0, board);
     print_board(board);

     printf("\n\nWhite Attacking:\n");
     show_sq_attacked_by_side(WHITE, board);

     printf("\n\nBlack Attacking:\n");
     show_sq_attacked_by_side(BLACK, board);

     //ASSERT(check_board(board));

     return 0;
}
