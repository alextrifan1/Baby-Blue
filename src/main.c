//
// Created by alex on 8/2/24.
//

#include <stdio.h>
#include "../include/defs.h"

int main() {
     AllInit();

     int index;
     for (index = 0; index < BOARD_SQ_NUMBER; index++) {
         if (index % 10 == 0)
             printf("\n");
         printf("%5d", square120_to_square64[index]);
     }

     printf("\n");
     printf("\n");
     for (index = 0; index < 64; index++) {
         if (index % 8 == 0)
             printf("\n");
         printf("%5d", square64_to_square120[index]);
     }
     return 0;
}
