//
// Created by alex on 8/5/24.
//

#include <stdio.h>
#include "defs.h"

/// check the integrity of our board structure
/// @param pos
/// @return always true if there is no problem
int check_board(const S_BOARD *pos) {
    int temp_piece_number[13] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int temp_big_piece[2] = {0, 0};
    int temp_major_piece[2] = {0, 0};
    int temp_minor_piece[2] = {0, 0};
    int temp_material[2] = {0, 0};

    int sq64, sq120, temp_piece, temp_piecenr, color, p_count;

    U64 temp_pawns[3] = {0ULL, 0ULL, 0ULL};

    temp_pawns[WHITE] = pos->pawns[WHITE];
    temp_pawns[BLACK] = pos->pawns[BLACK];
    temp_pawns[BOTH] = pos->pawns[BOTH];

    // check piece list
    for (temp_piece = wP; temp_piece <= bK; temp_piece++) {
        for (temp_piecenr = 0; temp_piecenr < pos->pieces_number[temp_piece]; temp_piecenr++) { //ex: we go through 8 wp
            sq120 = pos->p_list[temp_piece][temp_piecenr]; // we get the square for that piece
            ASSERT(pos->pieces[sq120] == temp_piece); //check if the piece on the array is the same as the temp_piece
        }
    }

    // check piece count and other counters
    for (sq64 = 0; sq64 < 64; ++sq64) {
        sq120 = SQ120(sq64);
        temp_piece = pos->pieces[sq120];
        temp_piece_number[temp_piece]++;
        color = piece_color[temp_piece];
        if (piece_big[temp_piece] == TRUE) temp_big_piece[color]++;
        if (piece_major[temp_piece] == TRUE) temp_major_piece[color]++;
        if (piece_minor[temp_piece] == TRUE) temp_minor_piece[color]++;

        temp_material[color] += piece_value[temp_piece];
    }

    for (temp_piece = wP; temp_piece <= bK; temp_piece++) {
        ASSERT(temp_piece_number[temp_piece] == pos->pieces_number[temp_piece]);
    }

    // check bitboard count
    p_count = COUNT(temp_pawns[WHITE]);
    ASSERT(p_count == pos->pieces_number[wP]);
    p_count = COUNT(temp_pawns[BLACK]);
    ASSERT(p_count == pos->pieces_number[bP]);
    p_count = COUNT(temp_pawns[BOTH]);
    ASSERT(p_count == (pos->pieces_number[wP] + pos->pieces_number[bP]));

    // check bitboard squares
    while (temp_pawns[WHITE]) {
        sq64 = POP(&temp_pawns[WHITE]);
        ASSERT(pos->pieces[SQ120(sq64)] == wP);
    }

    while (temp_pawns[BLACK]) {
        sq64 = POP(&temp_pawns[BLACK]);
        ASSERT(pos->pieces[SQ120(sq64)] == bP);
    }

    while (temp_pawns[BOTH]) {
        sq64 = POP(&temp_pawns[BOTH]);
        ASSERT((pos->pieces[SQ120(sq64)] == wP) || (pos->pieces[SQ120(sq64)] == bP));
    }

    ASSERT(temp_material[WHITE] == pos->material[WHITE] && temp_material[BLACK] == pos->material[BLACK]);
    ASSERT(temp_minor_piece[WHITE] == pos->minor_pieces[WHITE] && temp_minor_piece[BLACK] == pos->minor_pieces[BLACK]);
    ASSERT(temp_major_piece[WHITE] == pos->major_pieces[WHITE] && temp_major_piece[BLACK] == pos->major_pieces[BLACK]);
    ASSERT(temp_big_piece[WHITE] == pos->big_pieces[WHITE] && temp_big_piece[BLACK] == pos->big_pieces[BLACK])

    ASSERT(pos->side == WHITE || pos->side == BLACK);
    ASSERT(generate_position_key(pos) == pos->position_key);

    ASSERT(pos->en_passant == NO_SQ || (ranks_board[pos->en_passant] == RANK_6 && pos->side == WHITE)
           || (ranks_board[pos->en_passant] == RANK_3 && pos->side == BLACK));

    ASSERT(pos->pieces[pos->king_square[WHITE]] == wK);
    ASSERT(pos->pieces[pos->king_square[BLACK]] == bK);

    return TRUE;
}

/// updates nr of pieces, material and sets p_list
/// @param pos takes a position as the argument(our board structure)
void update_list_material(S_BOARD *pos) {
    int piece, sq, index, color;

    for (index = 0; index < BOARD_SQ_NUMBER; index++) {
        sq = index;
        piece = pos->pieces[index];
        if (piece != OFFBOARD && piece != EMPTY) {
            color = piece_color[piece];

            if (piece_big[piece] == TRUE) pos->big_pieces[color]++;
            if (piece_minor[piece] == TRUE) pos->minor_pieces[color]++;
            if (piece_major[piece] == TRUE) pos->major_pieces[color]++;

            pos->material[color] += piece_value[piece];

            //piece list ex: p_list[wP][pieces_number:0] = sq:a1
            pos->p_list[piece][pos->pieces_number[piece]] = sq;
            pos->pieces_number[piece]++;

            if (piece == wK) pos->king_square[WHITE] = sq;
            if (piece == bK) pos->king_square[BLACK] = sq;

            //TODO: check if the board setup is good
            if (piece == wP) {
                SETBIT(pos->pawns[WHITE], SQ64(sq)); // because sq is in the 124 bit format
                SETBIT(pos->pawns[BOTH], SQ64(sq));
            } else if (piece == bP) {
                SETBIT(pos->pawns[BLACK], SQ64(sq));
                SETBIT(pos->pawns[BOTH], SQ64(sq));
            }
        }
    }
}


int parse_fen(char *fen, S_BOARD *pos) {
    ASSERT(fen != NULL);
    ASSERT(pos != NULL);

    int rank = RANK_8; //in fen notation we start at rank 8(top of the borard)
    int file = FILE_A; // -||-           we start on file A
    int piece, count, i, sq64,sq120; //count is for empty squares

    reset_board(pos);

    while ((rank >= RANK_1) && *fen) {
        count = 1;
        switch (*fen) {
            case 'p': piece = bP; break;
            case 'r': piece = bR; break;
            case 'n': piece = bN; break;
            case 'b': piece = bB; break;
            case 'k': piece = bK; break;
            case 'q': piece = bQ; break;
            case 'P': piece = wP; break;
            case 'R': piece = wR; break;
            case 'N': piece = wN; break;
            case 'B': piece = wB; break;
            case 'K': piece = wK; break;
            case 'Q': piece = wQ; break;

            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
                piece = EMPTY;
                count = *fen - '0'; //can us atoi
                break;

            case '/':
            case ' ':
                rank--;
                file = FILE_A;
                fen++;
                continue;

            default:
                printf("FEN ERROR \n");
                return -1;

        }
        for (i = 0; i < count; i++) {
            sq64 = rank * 8 + file;
            sq120 = SQ120(sq64);
            if (piece != EMPTY) {
                pos->pieces[sq120] = piece;
            }
            file++;
        }
        fen++;
    }

    ASSERT(*fen == 'w' || *fen == 'b');

    pos->side = (*fen == 'w') ? WHITE : BLACK;
    fen += 2;//there is an empty space after w/b

    for (i = 0; i < 4; i++) {
        if (*fen == ' ')
            break;
        switch (*fen) {
            case 'K': pos->castle_permission |= WKCA; break;
            case 'Q': pos->castle_permission |= WQCA; break;
            case 'k': pos->castle_permission |= BKCA; break;
            case 'q': pos->castle_permission |= BQCA; break;
            default: break;
        }
        fen++;
    }
    fen++;

    ASSERT(pos->castle_permission >= 0 && pos->castle_permission <= 15);

    if (*fen != '-') {
        file = fen[0] - 'a';
        rank = fen[1] - '1';

        ASSERT(file >= FILE_A && file <= FILE_H);
        ASSERT(rank >= RANK_1 && rank <= RANK_8);

        pos->en_passant = FR2SQ(file,rank);
    }

    pos->position_key = generate_position_key(pos);
    update_list_material(pos);
    return 0;
}

/// the function should generate an empty board, all pieces, moves set to zero
/// @param pos our board structure
void reset_board(S_BOARD *pos) {
    int index;

    for (index = 0; index < BOARD_SQ_NUMBER; index++) {
        pos->pieces[index] = OFFBOARD;
    }
    for (index = 0; index < 64; index++) {
        pos->pieces[SQ120(index)] = EMPTY;
    }
    for(index = 0; index < 2; ++index) {
        pos->big_pieces[index] = 0;
        pos->major_pieces[index] = 0;
        pos->minor_pieces[index] = 0;
        pos->material[index] = 0;
    }

    for(index = 0; index < 3; ++index) {
        pos->pawns[index] = 0ULL;
    }

    for (index = 0; index < 13; index++) {
        pos->pieces_number[index] = 0;
    }

    pos->material[BLACK] = pos->material[WHITE] = 0; // this could have been a pain

    pos->king_square[WHITE] = pos->king_square[BLACK] = NO_SQ;

    pos->side = BOTH; //for handling errors later
    pos->en_passant = NO_SQ;
    pos->fifty_moves = 0;

    pos->ply = 0;
    pos->history_ply = 0;

    pos->castle_permission = 0;
    pos->position_key = 0ULL;

    pos->pv_table->p_table = NULL;                              /////////////----------------> if pointer errors, vid:51 comments
    init_pv_table(pos->pv_table);
}

void print_board(const S_BOARD *pos) {
    int sq, file, rank, piece;

    printf("\nGAME BOARD:\n\n");

    for (rank = RANK_8; rank >= RANK_1; rank--) {
        printf("%d ", rank + 1);//+1 because we use 0-indexing
        for (file = FILE_A; file <= FILE_H; file++) {
            sq = FR2SQ(file,rank);
            piece = pos->pieces[sq];
            printf("%3c",piece_char[piece]);
        }
        printf("\n");
    }
    printf("\n   ");
    for (file = FILE_A; file <= FILE_H; file++) {
        printf("%3c",'a' + file);
    }
    printf("\n");
    printf("side:%c\n", side_char[pos->side]);
    printf("en passant:%d\n", pos->en_passant);
    printf("castle:%c%c%c%c\n",
          pos->castle_permission & WKCA ? 'K' : '-',
          pos->castle_permission & WQCA ? 'Q' : '-',
          pos->castle_permission & BKCA ? 'k' : '-',
          pos->castle_permission & BQCA ? 'q' : '-'
          );
    printf("position key: %llX\n", pos->position_key);
}