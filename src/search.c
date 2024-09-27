//
// Created by alex on 9/19/24.
//

#include <math.h>
#include <stdio.h>
#include "defs.h"

#define MATE 29000

static void checkup() {
    // check if time up or stop from gui
}

//TODO: make this static and remove from defs.h
int is_repetition(const S_BOARD *pos) {
    int index;

    for (index = pos->history_ply - pos->fifty_moves; index < pos->history_ply; index++) {

        ASSERT(index >= 0 && index < MAX_GAME_MOVES);

        if (pos->position_key == pos->history[index].positon_key) {
            return TRUE;
        }
    }
    return FALSE;
}

static void clear_for_search(S_BOARD *pos, S_SEARCHINFO *info) {
    int index = 0;
    int index2 = 0;

    for (index = 0; index < 13; index++) {
        for (index2 = 0; index2 < BOARD_SQ_NUMBER; index2++) {
            pos->search_history[index][index2] = 0;
        }
    }

    for (index = 0; index < 2; index++) {
        for (index2 = 0; index2 < BOARD_SQ_NUMBER; index2++) {
            pos->search_killer[index][index2] = 0;
        }
    }
    clear_pv_table(pos->pv_table);
    pos->ply = 0;

    info->start_time = get_time_ms();
    info->stopped = 0;
    info->nodes = 0;
    info->fh = 0;
    info->fhf = 0;
}

static int quiescence(int alpha, int beta, S_BOARD *pos, S_SEARCHINFO *info) {
    return 0;
}

static int alpha_beta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int do_null) {
    ASSERT(check_board(pos));

    if (depth == 0) {
        info->nodes++;
        return evaluate_position(pos);
    }

    info->nodes++;

    if (is_repetition(pos) || pos->fifty_moves >= 100) {
        return 0;
    }

    if (pos->ply > MAXDEPTH -1) {
        return evaluate_position(pos);
    }

    S_MOVELIST list[1];
    generate_all_moves(pos, list);

    int move_nr = 0;
    int legal = 0;
    int old_alpha = alpha;
    int best_move = NOMOVE;
    int score = -INFINITE;

    for (move_nr = 0; move_nr < list->count; move_nr++) {
        if (!make_move(pos, list->moves[move_nr].move)) {
            continue;
        }
        legal++;
        score = -alpha_beta(-beta, -alpha, depth-1, pos, info, TRUE);
        take_move(pos);

        if (score > alpha) {
            if (score >= beta) {
                if (legal == 1) {
                    info->fhf++;
                }
                info->fh++;
                return beta;
            }
            alpha = score;
            best_move = list->moves[move_nr].move;
        }
    }

    if (legal == 0) {
        if (square_attacked(pos->king_square[pos->side], pos->side^1, pos)) {
            return -MATE + pos->ply;
        } else {
            return 0;
        }
    }

    if (alpha != old_alpha) {
        store_pv_move(pos, best_move);
    }

    return alpha;
}


void search_position(S_BOARD *pos, S_SEARCHINFO *info) {
    // iterative deepening, search init
    int best_move = NOMOVE;
    int best_score = -INFINITE;
    int current_depth = 0;
    int pv_moves = 0;
    int pv_number = 0;

    clear_for_search(pos, info);

    for (current_depth = 1; current_depth <= info->depth; current_depth++) {
        best_score = alpha_beta(-INFINITE, INFINITE, current_depth, pos, info, TRUE);
        pv_moves = get_pv_line(current_depth, pos);
        best_move = pos->pv_array[0];

        printf("Depth: %d, score: %d, move: %s, bodes: %ld ",
                current_depth, best_score, print_move(best_move), info->nodes);

        pv_moves = get_pv_line(current_depth, pos);
        printf("pv");
        for (pv_number = 0; pv_number < pv_moves; pv_number++) {
            printf(" %s", print_move(pos->pv_array[pv_number]));
        }
        printf("\n");
        printf("ordering: %.2f\n", (info->fhf/info->fh));
    }
}