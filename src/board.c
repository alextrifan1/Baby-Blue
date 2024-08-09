//
// Created by alex on 8/5/24.
//

#include <stdio.h>
#include "defs.h"

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

    pos->positon_key = generate_position_key(pos);
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
    for (index = 0; index < 3; index++) {
        if (index < 2) {
            pos->big_pieces[index] = 0;
            pos->major_pieces[index] = 0;
            pos->minor_pieces[index] = 0;
        }
        pos->pawns[index] = 0ULL;
    }

    for (index = 0; index < 13; index++) {
        pos->pieces_number[index] = 0;
    }

    pos->king_square[WHITE] = pos->king_square[BLACK] = NO_SQ;

    pos->side = BOTH; //for handling errors later
    pos->en_passant = NO_SQ;
    pos->fifty_moves = 0;

    pos->ply = 0;
    pos->history_ply = 0;

    pos->castle_permission = 0;
    pos->positon_key = 0ULL;
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
    printf("position key: %llX\n", pos->positon_key);
}