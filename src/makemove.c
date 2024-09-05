//
// Created by alex on 8/14/24.
//

#include <defs.h>
#include <stdio.h>
#include <strings.h>

#define HASH_PIECE(piece, square) (pos->position_key ^= (piece_keys[(piece)][(square)]))
#define HASH_CASTLE (pos->position_key ^= (castle_key[(pos->castle_permission)]))
#define HASH_SIDE (pos->position_key ^= (side_key))
#define HASH_ENPAS (pos->position_key ^= (piece_keys[EMPTY][pos->en_passant]))

const int castle_permission[120] = {
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 13, 15, 15, 15, 12, 15, 15, 14, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15,  7, 15, 15, 15,  3, 15, 15, 11, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15,
    15, 15, 15, 15, 15, 15, 15, 15, 15, 15
};

/// clear a piece from given square
static void clear_piece(const int sq, S_BOARD *pos) {

    ASSERT(sq_on_board(sq));

    int piece = pos->pieces[sq];

    ASSERT(piece_valid(piece));

    int color = piece_color[piece];
    int index = 0;
    int t_piece_number = -1;

    HASH_PIECE(piece, sq);

    pos->pieces[sq] = EMPTY;
    pos->material[color] -= piece_value[piece];

    if (piece_big[piece]) {
        pos->big_pieces[color]--;
        if (piece_major[piece]) {
            pos->major_pieces[color]--;
        } else {
            pos->minor_pieces[color]--;
        }
    } else {
        CLEARBIT(pos->pawns[color], SQ64(sq));
        CLEARBIT(pos->pawns[BOTH], SQ64(sq));
    }

    /*  EXAMPLE
     *  pos->pieces_number[wP] == 5 -> loop from 0 to 4
     *
     *  pos->p_list[piece][0] == sq0
     *  pos->p_list[piece][1] == sq1
     *  pos->p_list[piece][2] == sq2
     *  pos->p_list[piece][3] == sq3
     *  pos->p_list[piece][4] == sq4
     *
     *  sq == sq3 so t_piece_number = 3
     */
    for (index = 0; index < pos->pieces_number[piece]; index++) {
        if (pos->p_list[piece][index] == sq) {
            t_piece_number = index;
            break;
        }
    }

    ASSERT(t_piece_number != -1);

    pos->pieces_number[piece]--;
    // pos->pieces_number[wP] == 4
    pos->p_list[piece][t_piece_number] = pos->p_list[piece][pos->pieces_number[piece]];
    //pos->p_list[wP][3] = pos->p_list[wP][4] = sq4

    /*  pos->pieces_number[wP] == 4 -> loop from 0 to 3
     *
     *  pos->p_list[piece][0] == sq0
     *  pos->p_list[piece][1] == sq1
     *  pos->p_list[piece][2] == sq2
     *  pos->p_list[piece][3] == sq4
     *
     *  we no longer have sq3 as a value where there is a piece in our piece list
     */
}

static void add_piece(const int sq, S_BOARD *pos, const int piece) {
    ASSERT(piece_valid(piece));
    ASSERT((sq_on_board(sq)));
    int color = piece_color[piece];
    ASSERT(side_valid(color));

    HASH_PIECE(piece, sq);
    pos->pieces[sq] = piece;
    if (piece_big[piece]) {
        pos->big_pieces[color]++;
        if (piece_major[piece]) {
            pos->major_pieces[color]++;
        } else {
            pos->minor_pieces[color]++;
        }
    } else {
        SETBIT(pos->pawns[color], SQ64(sq));
        SETBIT(pos->pawns[BOTH], SQ64(sq));
    }
    pos->material[color] += piece_value[piece];
    pos->p_list[piece][pos->pieces_number[piece]++] = sq;
}

static void move_piece(const int from, const int to, S_BOARD *pos) {
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    int index = 0;
    int piece = pos->pieces[from];
    int color = piece_color[piece];
    ASSERT(side_valid(color));
    ASSERT(piece_valid(piece));

    #ifdef DEBUG
        int t_piece_number = FALSE;
    #endif

    HASH_PIECE(piece,from);
    pos->pieces[from] = EMPTY;

    HASH_PIECE(piece,to);
    pos->pieces[to] = piece;

    if(!piece_big[piece]) {
        CLEARBIT(pos->pawns[color],SQ64(from));
        CLEARBIT(pos->pawns[BOTH],SQ64(from));
        SETBIT(pos->pawns[color],SQ64(to));
        SETBIT(pos->pawns[BOTH],SQ64(to));
    }

    for(index = 0; index < pos->pieces_number[piece]; index++) {
        if(pos->p_list[piece][index] == from) {
            pos->p_list[piece][index] = to;
            #ifdef DEBUG
                t_piece_number = TRUE;
            #endif
            break;
        }
    }
    ASSERT(t_piece_number);
}

///
/// @param pos
/// @param move
/// @return 0 if the side that moved left herself in check or 1 if move legal
int make_move(S_BOARD *pos, int move) {

    ASSERT(check_board(pos));

    int from = FROMSQ(move);
    int to = TOSQ(move);
    int side = pos->side;
    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));
    ASSERT(side_valid(side));
    ASSERT(piece_valid(pos->pieces[from]));

    pos->history[pos->history_ply].positon_key = pos->position_key;

    if (move & MFLAGEP) {
        if (side == WHITE) {
            clear_piece(to - 10, pos);
        } else {
            clear_piece(to+10, pos);
        }
    } else if (move & MFLAGCA) {
        switch (to) {
            case C1:
                move_piece(A1, D1, pos);
            break;
            case C8:
                move_piece(A8, D8, pos);
            break;
            case G1:
                move_piece(H1, F1, pos);
            break;
            case G8:
                move_piece(H8, F8, pos);
            break;
            default: ASSERT(FALSE); break;
        }
    }

    if (pos->en_passant != NO_SQ)
        HASH_ENPAS;
    HASH_CASTLE;

    pos->history[pos->history_ply].move = move;
    pos->history[pos->history_ply].fifty_move = pos->fifty_moves;
    pos->history[pos->history_ply].en_passant = pos->en_passant;
    pos->history[pos->history_ply].castle_permission = pos->castle_permission;

    pos->castle_permission &= castle_permission[from];
    pos->castle_permission &= castle_permission[to];
    pos->en_passant = NO_SQ;

    HASH_CASTLE;

    int captured = CAPTURED(move);
    pos->fifty_moves++;

    if (captured != EMPTY) {
        ASSERT(piece_valid(captured));
        clear_piece(to, pos);
        pos->fifty_moves = 0;
    }

    pos->history_ply++;
    pos->ply++;

    ASSERT(pos->history_ply >= 0 && pos->history_ply < MAX_GAME_MOVES);
    //ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

    if(piece_pawn[pos->pieces[from]]) {
        pos->fifty_moves = 0;
        if(move & MFLAGPS) {
            if(side==WHITE) {
                pos->en_passant=from+10;
                ASSERT(ranks_board[pos->en_passant]==RANK_3);
            } else {
                pos->en_passant=from-10;
                ASSERT(ranks_board[pos->en_passant]==RANK_6);
            }
            HASH_ENPAS;
        }
    }

    move_piece(from, to, pos);

    int promoted_piece = PROMOTED(move);
    if (promoted_piece != EMPTY) {
        ASSERT(piece_valid(promoted_piece) && !piece_pawn[promoted_piece]);
        clear_piece(to, pos);
        add_piece(to, pos, promoted_piece);
    }

    if (piece_king[pos->pieces[to]]) {
        pos->king_square[pos->side] = to;
    }

    pos->side ^= 1;
    HASH_SIDE;

    ASSERT(check_board(pos));

    if (square_attacked(pos->king_square[side], pos->side, pos)) {
        take_move(pos);
        return FALSE;
    }

    return TRUE;
}

void take_move(S_BOARD *pos) {

    ASSERT(check_board(pos));

    pos->history_ply--;
    pos->ply--;

    ASSERT(pos->history_ply >= 0 && pos->history_ply < MAX_GAME_MOVES);
    //ASSERT(pos->ply >= 0 && pos->ply < MAXDEPTH);

    int move = pos->history[pos->history_ply].move;
    int from = FROMSQ(move);
    int to = TOSQ(move);

    ASSERT(sq_on_board(from));
    ASSERT(sq_on_board(to));

    if(pos->en_passant != NO_SQ)
        HASH_ENPAS;
    HASH_CASTLE;

    pos->castle_permission = pos->history[pos->history_ply].castle_permission;
    pos->fifty_moves = pos->history[pos->history_ply].fifty_move;
    pos->en_passant = pos->history[pos->history_ply].en_passant;

    if(pos->en_passant != NO_SQ)
        HASH_ENPAS;
    HASH_CASTLE;

    pos->side ^= 1;
    HASH_SIDE;

    if(MFLAGEP & move) {
        if(pos->side == WHITE) {
            add_piece(to - 10, pos, bP);
        } else {
            add_piece(to + 10, pos, wP);
        }
    } else if(MFLAGCA & move) {
        switch(to) {
            case C1:
                move_piece(D1, A1, pos);
            break;
            case C8:
                move_piece(D8, A8, pos);
            break;
            case G1:
                move_piece(F1, H1, pos);
            break;
            case G8:
                move_piece(F8, H8, pos);
            break;
            default: ASSERT(FALSE); break;
        }
    }

    move_piece(to, from, pos);

    if(piece_king[pos->pieces[from]]) {
        pos->king_square[pos->side] = from;
    }

    int captured = CAPTURED(move);
    if(captured != EMPTY) {
        ASSERT(piece_valid(captured));
        add_piece(to, pos, captured);
    }

    if(PROMOTED(move) != EMPTY)   {
        ASSERT(piece_valid(PROMOTED(move)) && !piece_valid(PROMOTED(move)));
        clear_piece(from, pos);
        add_piece(from, pos, (piece_color[PROMOTED(move)] == WHITE ? wP : bP));
    }

    ASSERT(check_board(pos));
}