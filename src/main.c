//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include "../include/defs.h"

int main() {
     all_init();

     U64 playBitBoard = 0ULL;

     SETBIT(playBitBoard, 17);
     print_bitboard(playBitBoard);

     CLEARBIT(playBitBoard, 17);
     print_bitboard(playBitBoard);

     return 0;
}
