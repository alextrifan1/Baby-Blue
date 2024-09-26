//
// Created by alex on 9/5/24.
//

#include <assert.h>
#include <defs.h>
#include <stdio.h>

long leaf_nodes;


void perft(int depth, S_BOARD *pos) {
    ASSERT(check_board(pos));

    if (depth == 0) {
        leaf_nodes++;
        return;
    }

    S_MOVELIST list[1];
    generate_all_moves(pos, list);

    int move_nr = 0;
    for (move_nr = 0; move_nr < list->count; move_nr++) {
        if (!make_move(pos, list->moves[move_nr].move)) {
            continue;
        }
        perft(depth - 1, pos);
        take_move(pos);
    }

    return;
}

void perf_test(int depth, S_BOARD *pos) {
    ASSERT(check_board(pos));

    print_board(pos);
    printf("\nStarting Test To Depth: %d\n", depth);
    leaf_nodes = 0;
    int start = get_time_ms();

    S_MOVELIST list[1];
    generate_all_moves(pos, list);

    int move;
    int move_nr = 0;
    for (move_nr = 0; move_nr < list->count; move_nr++) {
        move = list->moves[move_nr].move;
        if (!make_move(pos, move)) {
            continue;
        }
        long cum_nodes = leaf_nodes;
        perft(depth - 1, pos);
        take_move(pos);
        long old_nodes = leaf_nodes - cum_nodes;
        printf("move %d : %s : %ld\n", move_nr+1, print_move(move), old_nodes);
    }

    printf("\nTest Complete : %ld nodes visited in %dms\n", leaf_nodes, get_time_ms() - start);

    return;

}