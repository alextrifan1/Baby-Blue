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

static void pick_next_move(int move_nr, S_MOVELIST *list) {
    S_MOVE temp;
    int index = 0;
    int best_score = 0;
    int best_nr = move_nr;

    for (index = move_nr; index < list->count; index++) {
        if (list->moves[index].score > best_score) {
            best_score = list->moves[index].score;
            best_nr = index;
        }
    }
    temp = list->moves[move_nr];
    list->moves[move_nr] = list->moves[best_nr];
    list->moves[best_nr] = temp;
}

//TODO: make this static and remove from defs.h (maybe)
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

    ASSERT(check_board(pos));
    info->nodes++;

    if (is_repetition(pos) || pos->fifty_moves >= 100) {
        return 0;
    }
    if (pos->ply > MAXDEPTH - 1) {
        return evaluate_position(pos);
    }

    int score = evaluate_position(pos);

    if (score >= beta) {
        return beta;
    }
    if (score > alpha) {
        alpha = score;
    }

    S_MOVELIST list[1];
    generate_all_captures(pos, list);

    int move_nr = 0;
    int legal = 0;
    int old_alpha = alpha;
    int best_move = NOMOVE;
    score = -INFINITE;
    int pv_move = probe_pv_table(pos);

    for (move_nr = 0; move_nr < list->count; move_nr++) {

        pick_next_move(move_nr, list);

        if (!make_move(pos, list->moves[move_nr].move)) {
            continue;
        }
        legal++;
        score = -quiescence(-beta, -alpha, pos, info);
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

    if (alpha != old_alpha) {
        store_pv_move(pos, best_move);
    }
    return alpha;
}

static int alpha_beta(int alpha, int beta, int depth, S_BOARD *pos, S_SEARCHINFO *info, int do_null) {
    ASSERT(check_board(pos));

    if (depth == 0) {
        return quiescence(alpha, beta, pos, info);
        //return evaluate_position(pos);
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
    int pv_move = probe_pv_table(pos);

    if (pv_move != NOMOVE) {
        for (move_nr = 0; move_nr < list->count; move_nr++) {
            if (list->moves[move_nr].move == pv_move) {
                list->moves[move_nr].score = 2000000;
                break;
            }
        }
    }

    for (move_nr = 0; move_nr < list->count; move_nr++) {

        pick_next_move(move_nr, list);

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

                if (!(list->moves[move_nr].move & MFLAGCAP)) {
                    pos->search_killer[1][pos->ply] = pos->search_killer[0][pos->ply];
                    pos->search_killer[0][pos->ply] = list->moves[move_nr].move;
                }

                return beta;
            }
            alpha = score;
            best_move = list->moves[move_nr].move;
            if (!(list->moves[move_nr].move & MFLAGCAP)) {
                pos->search_history[pos->pieces[FROMSQ(best_move)]][TOSQ(best_move)] += depth;
            }
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