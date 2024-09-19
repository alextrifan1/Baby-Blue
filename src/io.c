//
// Created by alex on 8/12/24.
//

#include <stdio.h>
#include "../include/defs.h"

/// prints the algebraic notation for the square
/// @param sq square
/// @return algebraic notation for the square
char *print_square(const int sq) {
    static char sq_string[3];

    int file = files_board[sq];
    int rank = ranks_board[sq];

    sprintf(sq_string, "%c%c", ('a' + file), ('1' + rank));

    return sq_string;
}

/// prints a move in algebraic notation
/// @param move
/// @return algebraic notation for the move
char *print_move(const int move) {
    static  char move_string[6];
    int file_from = files_board[FROMSQ(move)];
    int rank_from = ranks_board[FROMSQ(move)];
    int file_to = files_board[TOSQ(move)];
    int rank_to = ranks_board[TOSQ(move)];

    int promoted = PROMOTED(move);

    if (promoted) {
        char p_char = 'q'; //assume the promoted piece is a queen
        if (IsKn(promoted)) {
            p_char = 'n';
        } else if (IsRQ(promoted) && !IsBQ(promoted)) {
            p_char = 'r';
        } else if (!IsRQ(promoted) && IsBQ(promoted)) {
            p_char = 'b';
        }
        sprintf(move_string, "%c%c%c%c%c", ('a' + file_from), ('1' + rank_from), ('a' + file_to), ('1' + rank_to), p_char);
    } else {
        sprintf(move_string, "%c%c%c%c", ('a' + file_from), ('1' + rank_from), ('a' + file_to), ('1' + rank_to));
    }

    return move_string;
}

/// take a move from user
/// @param ptr_char -move
/// @param pos - board structure
/// @return
int parse_move(char* ptr_char, S_BOARD* pos) {
    ASSERT(check_board(pos));

    if(ptr_char[1] > '8' || ptr_char[1] < '1') return NOMOVE;
    if(ptr_char[3] > '8' || ptr_char[3] < '1') return NOMOVE;
    if(ptr_char[0] > 'h' || ptr_char[0] < 'a') return NOMOVE;
    if(ptr_char[2] > 'h' || ptr_char[2] < 'a') return NOMOVE;

    int from = FR2SQ(ptr_char[0] - 'a', ptr_char[1] - '1');
    int to = FR2SQ(ptr_char[2] - 'a', ptr_char[3] - '1');

    ASSERT(sq_on_board(from) && sq_on_board(to));

    S_MOVELIST list[1];
    generate_all_moves(pos, list);
    int move_nr = 0;
    int move = 0;
    int promoted_piece = EMPTY;

    for(move_nr = 0; move_nr < list->count; move_nr++) {
        move = list->moves[move_nr].move;
        if(FROMSQ(move)==from && TOSQ(move)==to) {
            promoted_piece = PROMOTED(move);
            if(promoted_piece!=EMPTY) { // see to what we promoted
                if(IsRQ(promoted_piece) && !IsBQ(promoted_piece) && ptr_char[4]=='r') {
                    return move;
                } else if(!IsRQ(promoted_piece) && IsBQ(promoted_piece) && ptr_char[4]=='b') {
                    return move;
                } else if(IsRQ(promoted_piece) && IsBQ(promoted_piece) && ptr_char[4]=='q') {
                    return move;
                } else if(IsKn(promoted_piece) && ptr_char[4]=='n') {
                    return move;
                }
                continue;
            }
            return move;
        }
    }

    return NOMOVE;

}

void print_movelist(const S_MOVELIST *list) {
    int index = 0, score = 0, move = 0;

    printf("MoveList:%d\n", list->count);

    for (index = 0; index < list->count; index++) {
        move = list->moves[index].move;
        score = list->moves[index].score;

        printf("Move:%d > %s (score:%d)\n", index+1, print_move(move), score);
    }
    printf("MoveList Total %d Moves:\n\n", list->count);
}