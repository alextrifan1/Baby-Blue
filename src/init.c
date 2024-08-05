//
// Created by alex on 8/2/24.
//

#include "../include/defs.h"
#include <stdlib.h>

//this macro helps us generate a random 64bit number that uses all the 64 bits
#define RAND_64 ( (U64)rand() + ((U64)rand() << 15) + ((U64)rand() << 30) + ((U64)rand() << 45) + (((U64)rand() & 0xf) << 60) )

int square120_to_square64[BOARD_SQ_NUMBER];
int square64_to_square120[64];

U64 set_mask[64];
U64 clear_mask[64];

U64 piece_keys[13][120];
U64 side_key;
U64 castle_key[16];

void init_hashkeys() {
    for (int index = 0; index < 13; index++) {
        for (int index2 = 0; index2 < 120; index2++) {
            piece_keys[index][index2] = RAND_64;
        }
    }
    side_key = RAND_64;
    for (int index = 0; index < 16; index++) {
        castle_key[index] = RAND_64;
    }
}

void init_bitmasks() {
    for (int index = 0; index < 64; index++) {
        set_mask[index] = 0ULL;
        clear_mask[index] = 0ULL;
    }
    for (int index = 0; index < 64; index++) {
        set_mask[index] |= (1ULL << index);
        clear_mask[index] = ~set_mask[index];
    }
}

void init_sq120_to_sq64() {
    int index, sq, sq64 = 0;

    //init with imposible values
    for (index = 0; index < BOARD_SQ_NUMBER; index++) {
        square120_to_square64[index] = 65;
    }

    for (index = 0; index < 64; index++) {
        square64_to_square120[index] = 120;
    }

    //setting up values
    //sq64 is used as an index for the 64base board
    for (int rank = RANK_1; rank <= RANK_8; rank++) {
        for (int file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file, rank);
            square64_to_square120[sq64] = sq;
            square120_to_square64[sq] = sq64;
            sq64++;
        }
    }

}

void all_init() {
    init_sq120_to_sq64();
    init_bitmasks();
    init_hashkeys();
}