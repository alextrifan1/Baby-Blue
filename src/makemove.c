//
// Created by alex on 8/14/24.
//

#include <defs.h>
#include <stdio.h>
#include <strings.h>

#define HASH_PIECE(piece, square) (pos->positon_key ^= (piece_keys[(piece)][(square)]))
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