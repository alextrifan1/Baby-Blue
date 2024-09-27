//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include <stdlib.h>
#include "../include/defs.h"

#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R w KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 w - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 w - - 0 1"
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"
#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE2 "3rk2r/8/8/8/8/8/6p1/R3K2R b KQk - 0 1"
#define TESTMOVE "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define HARD_FEN "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define PERFTFEN "n1n5/PPPk4/8/8/8/8/4Kppp/5N1N w - - 0 1"
#define MATE3 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define GOOD "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

int main() {
     all_init();

     S_BOARD board[1];
     board->pv_table->p_table = NULL;                              /////////////----------------> if pointer errors, vid:51 comments
     init_pv_table(board->pv_table);
     S_MOVELIST list[1];
     S_SEARCHINFO info[1];

     parse_fen(GOOD, board);
     //perf_test(3, board);

     char input[6];
     int move = NOMOVE;
     int pv_num = 0;
     int max = 0;
     while (TRUE) {
          print_board(board);
          printf("please enter move > ");
          fgets(input, 6, stdin);

          if (input[0] == 'q') {
               break;
          } else if (input[0] == 't') {
               take_move(board);
          } else if (input[0] == 's') {
               info->depth = 5;
               search_position(board, info);

          } else {
               move = parse_move(input, board);
               if (move != NOMOVE) {
                    store_pv_move(board, move);
                    make_move(board, move);
                    /*if (is_repetition(board)) {
                         printf("REP SEEN\n");
                    }*/
               } else {
                    printf("Move not parsed %s\n", input);
               }
          }

          fflush(stdin);
     }

     free(board->pv_table->p_table);

     return 0;
}


