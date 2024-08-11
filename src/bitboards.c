//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include "../include/defs.h"

const int bit_table[64] = {
    63, 30, 3, 32, 25, 41, 22, 33, 15, 50, 42, 13, 11, 53, 19, 34, 61, 29, 2,
    51, 21, 43, 45, 10, 18, 47, 1, 54, 9, 57, 0, 35, 62, 31, 40, 4, 49, 5, 52,
    26, 60, 6, 23, 44, 46, 27, 56, 16, 7, 39, 48, 24, 59, 14, 12, 55, 38, 28,
    58, 20, 37, 17, 36, 8
  };

int pop_bit(U64 *bb) {
    U64 b = *bb ^ (*bb - 1);
    unsigned int fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    *bb &= (*bb - 1);
    return bit_table[(fold * 0x783a9b23) >> 26];
}

int count_bits(U64 b) {
    int nr;
    for (nr = 0; b; nr++, b &= b - 1);
    return nr;
}

void print_bitboard(U64 bb) {
    U64 shift_me = 1ULL; //unsigned long long bit

    int sq = 0;
    int sq64 = 0;

    printf("\n");
    for (int rank = RANK_8; rank >= RANK_1; rank--) {
        for (int file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank); // 120 based index
            sq64 = SQ64(sq);    //64 based index
            //we shift 1(64 bit representation) with sq64 positions and check if it matches with bb
            if ((shift_me << sq64) & bb)
                printf("X");
            else
                printf("-");
        }
        printf("\n");
    }
    printf("\n\n");
}

