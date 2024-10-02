//
// Created by alex on 10/2/24.
//

#include <stdio.h>
#include <defs.h>
#include <string.h>

#define INPUTBUFFER 400 * 6

void parse_go(char* line, S_SEARCHINFO *info, S_BOARD *pos);
void parse_position(char* line_in, S_BOARD *pos);

void uci_loop() {
    setbuf(stdin, NULL);
    setbuf(stdout,NULL);

    char line[INPUTBUFFER];
    printf("id name %s\n", NAME);
    printf("id author Alex Trifan\n");
    printf("uciok\n");

    S_BOARD pos[1];
    S_SEARCHINFO info[1];
    init_pv_table(pos->pv_table);

    while (TRUE) {
        memset(&line[0], 0, sizeof(line));
        fflush(stdout);
        if (!fgets(line, INPUTBUFFER, stdin)) {
            continue;
        }
        if (line[0] == '\n')
            continue;

        if (!strncmp(line, "isready", 7)) {
            printf("readyok");
            continue;
        } else if (!strncmp(line, "position", 8)) {
            parse_position(line, pos);
        } else if (!strncmp(line, "ucinewgame", 10)) {
            parse_position("position startpos\n", pos);
        } else if (!strncmp(line, "go", 3)) {
            parse_go(line, info, pos);
        } else if (!strncmp(line, "quit", 4)) {
            info->quit = TRUE;
            break;
        } else if (!strncmp(line, "uci", 3)) {
            printf("id name %s\n", NAME);
            printf("id author Alex Trifan\n");
            printf("uciok\n");
        }
        if (info->quit) break;
    }
}

void parse_go(char* line, S_SEARCHINFO *info, S_BOARD *pos) {

}

void parse_position(char* line_in, S_BOARD *pos) {

}

