//
// Created by alex on 9/21/24.
//


#include <stdio.h>
#include "defs.h"

const int pv_size = 0x100000 * 2; //2mb

int get_pv_line(const int depth, S_BOARD *pos) {
    ASSERT(depth < MAXDEPTH);

    int move = probe_pv_table(pos);
    int count = 0;

    while (move != NOMOVE && count < depth) {
        ASSERT(count < MAXDEPTH);

        if (move_exists(pos, move)) {
            make_move(pos, move);
            pos->pv_array[count++] = move;
        } else {
            break;
        }
        move = probe_pv_table(pos);
    }

    while (pos->ply > 0) {
        take_move(pos);
    }

    return count;

}

void clear_pv_table(S_PVTABLE *table) {
    S_PVENTRY *pv_entry;

    for (pv_entry = table->p_table; pv_entry < table->p_table + table->nr_entries; pv_entry++) {
        pv_entry->position_key = 0ULL;
        pv_entry->move = NOMOVE;
    }
}

void init_pv_table(S_PVTABLE *table) {
    table->nr_entries = pv_size/sizeof(S_PVENTRY);
    table->nr_entries -= 2;
    free(table->p_table);
    table->p_table = (S_PVENTRY*)malloc(table->nr_entries * sizeof(S_PVENTRY));
    clear_pv_table(table);
    printf("pv table complete with %d entries\n", table->nr_entries);
}

void store_pv_move(const S_BOARD *pos, const int move) {
    int index = pos->position_key % pos->pv_table->nr_entries;

    ASSERT(index >= 0 && index <= pos->pv_table->nr_entries - 1);

    pos->pv_table->p_table[index].move = move;
    pos->pv_table->p_table[index].position_key = pos->position_key;
}

int probe_pv_table(const S_BOARD *pos) {
    int index = pos->position_key % pos->pv_table->nr_entries;

    ASSERT(index >= 0 && index <= pos->pv_table->nr_entries - 1);


    if (pos->pv_table->p_table[index].position_key == pos->position_key)
        return pos->pv_table->p_table[index].move;

    return NOMOVE;

}