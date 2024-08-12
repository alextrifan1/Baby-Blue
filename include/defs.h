//
// Created by alex on 8/2/24.
//

#ifndef DEFS_H
#define DEFS_H

#include <stdlib.h>

#define DEBUG

#ifndef DEBUG
#define ASSERT(n)
#else
#define ASSERT(n) \
if (!(n)) { \
printf("%s - Failed ", #n); \
printf("On %s ", __DATE__); \
printf("At %s ", __TIME__); \
printf("In File %s ", __FILE__); \
printf("At Line %d\n", __LINE__); \
exit(1); }
#endif


typedef unsigned long long U64;

#define NAME "BABY-BLUE 1.0"
#define BOARD_SQ_NUMBER 120

#define MAX_GAME_MOVES 2048 //half moves
#define MAX_POSITION_MOVES 256

#define START_FEN "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"

enum { EMPTY, wP, wN, wB, wR, wQ, wK, bP, bN, bB, bR, bQ, bK}; //w-white b-black, P-pawn...
enum { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H, FILE_NONE}; //board horizontal
enum { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8, RANK_NONE}; //board vertical
enum { WHITE, BLACK, BOTH }; //colors we use

/* see board representation */
enum {
    A1 = 21, B1, C1, D1, E1, F1, G1, H1,
    A2 = 31, B2, C2, D2, E2, F2, G2, H2,
    A3 = 41, B3, C3, D3, E3, F3, G3, H3,
    A4 = 51, B4, C4, D4, E4, F4, G4, H4,
    A5 = 61, B5, C5, D5, E5, F5, G5, H5,
    A6 = 71, B6, C6, D6, E6, F6, G6, H6,
    A7 = 81, B7, C7, D7, E7, F7, G7, H7,
    A8 = 91, B8, C8, D8, E8, F8, G8, H8, NO_SQ, OFFBOARD
};

enum { FALSE, TRUE};

typedef struct {
    int move;
    int score;
} S_MOVE;

typedef struct {
    S_MOVE moves[MAX_POSITION_MOVES];
    int count;
} S_MOVELIST;

/*  WKCA: white king king side casteling
    think of them as bits
    1 0 0 1 - white king can castle on king side, black king can castle on queen side
*/
enum { WKCA = 1, WQCA = 2, BKCA = 4, BQCA = 8};

typedef struct {
    int move;
    int castle_permission;
    int en_passant; //the en passant square if there is one
    int fifty_move; //counter for making a forced draw
    U64 positon_key;
} S_UNDO;

typedef struct {
    int pieces[BOARD_SQ_NUMBER];
    U64 pawns[3]; // [0,1,2] 0-if there is no pawn with the same color, 1 if there is

    int king_square[2];

    int side; //current side to move
    int castle_permission;
    int en_passant; //the en passant square if there is one
    int fifty_moves; //counter for making a forced draw

    int ply; //half move
    int history_ply; // index: how many half moves were made
    U64 positon_key;

    int pieces_number[13];
    int big_pieces[2]; //anything but pawns
    int major_pieces[2]; //rooks, queens
    int minor_pieces[2]; //bishops, knights
    int material[2]; //for calculating the value of the pieces gained

    S_UNDO history[MAX_GAME_MOVES];

    // piece list -- for time savings
    int p_list[13][10];
} S_BOARD;

/* GAME MOVE */

/*
 This is explaining how in one intiger we ar keeping various atributes of our S_MOVE move

    we use the bits in the following manner:

    0000 0000 0000 0000 0000 0111 1111 - move: from                         |   (7F)      ->     0x7F
    0000 0000 0000 0011 1111 1000 0000 - move: to                           |   (3F80)    ->     >> 0x7F
    0000 0000 0011 1100 0000 0000 0000 - move: captured(if any)             |   (3C000)   ->     >> 0xF
    0000 0000 0100 0000 0000 0000 0000 - move: was it a en passant move ?   |   (40000)   ->     0x40000
    0000 0000 1000 0000 0000 0000 0000 - move: pawn start                   |   (80000)   ->     0x80000
    0000 1111 0000 0000 0000 0000 0000 - move: promoted piece(if any)       |   (F00000)  ->     >>20 0xF
    0001 0000 0000 0000 0000 0000 0000 - move: was it a castle move ?       |   (1000000) ->     0x1000000

    we'll use hexadecimal notation

*/

/* MACROS */

//given a file(f) and rank(r) return the equivalent square in the array
#define FR2SQ(f,r) ( (21 + (f) ) + ( (r) * 10 ) )
#define SQ64(sq120) (square120_to_square64[(sq120)])
#define SQ120(sq64) (square64_to_square120[(sq64)])
#define POP(b) pop_bit(b)
#define COUNT(b) count_bits(b)
#define CLEARBIT(bb, sq) ((bb) &= clear_mask[(sq)])
#define SETBIT(bb,sq) ((bb) |= set_mask[(sq)])

#define IsBQ(p) (piece_bishop_queen[(p)])
#define IsRQ(p) (piece_rook_queen[(p)])
#define IsKn(p) (piece_knight[(p)])
#define IsKi(p) (piece_king[(p)])

#define FROMSQ(m) ((m) & 0x7F)
#define TOSQ(m) (((m)>>7) & 0x7F)
#define CAPTURED(m) (((m)>>14) & 0xF)
#define PROMOTED(m) (((m)>>20) & 0xF)

#define MFLAGEP 0x40000
#define MFLAGPS 0x80000
#define MFLAGCA 0x1000000

#define MFLAGCAP 0x7C000
#define MFLAGPROM 0xF00000

/* GLOBALS */

extern int square120_to_square64[BOARD_SQ_NUMBER];
extern int square64_to_square120[64];
extern U64 set_mask[64];
extern U64 clear_mask[64];
extern U64 piece_keys[13][120];
extern U64 side_key;
extern U64 castle_key[16];
extern char piece_char[];
extern char side_char[];
extern char rank_char[];
extern char file_char[];

extern int piece_big[13];
extern int piece_major[13];
extern int piece_minor[13];
extern int piece_value[13];
extern int piece_color[13];

extern int files_board[BOARD_SQ_NUMBER];
extern int ranks_board[BOARD_SQ_NUMBER];

//we'll use this to see if a piece is a % as we iterate through the board
extern int piece_knight[13];
extern int piece_king[13];
extern int piece_rook_queen[13];
extern int piece_bishop_queen[13];

/* FUNCTIONS */

// init.c
extern void all_init();

// bitboard.c
extern void print_bitboard(U64 bb);
extern int pop_bit(U64 *bb);
extern int count_bits(U64 b);

// hashkeys.c
extern U64 generate_position_key(const S_BOARD *pos);

// board.c
extern void reset_board(S_BOARD *pos);
extern int parse_fen(char *fen, S_BOARD *pos);
extern void print_board(const S_BOARD *pos);
extern void update_list_material(S_BOARD *pos);
extern int check_board(const S_BOARD *pos);

// attack.c
extern int square_attacked(const int sq, const int side, const S_BOARD *pos);

// io.c
extern char *print_square(const int sq);
extern char *print_move(const int move);
extern void print_movelist(const S_MOVELIST *list);

// validate.c
extern int sq_on_board(const int sq);
extern int side_valid(const int side);
extern int file_rank_valid(const int fr);
extern int piece_valid_empty(const int pce);
extern int piece_valid(const int pce);

// movgen.c
extern void generate_all_moves(const S_BOARD *pos, S_MOVELIST *list);

#endif //DEFS_H
