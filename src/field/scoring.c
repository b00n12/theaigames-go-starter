/*
 * Copyright 2016 (C)
 * Jan Ruh
 * 
 * gostarter is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * gostarter is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with gostarter. If not, see <http://www.gnu.org/licenses/>.
 */

#include "scoring.h"
#include "../utlist.h"
#include "../bot/bot_state.h"
#include "../field/field.h"
#include "../util.h"

#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define KOMI 7.5

typedef struct Cell {
    unsigned int x_coordinate;
    unsigned int y_coordinate;
    short value;

    /*
     * Needed for usage of utlist
     */
    struct Cell *prev;
    struct Cell *next;
} Cell;

typedef struct DFSState {
    short **field;
    bool **visited;

    Cell *reached_list;
    Cell *matched_list;
} DFSState;

static short cmp_cells_coordinates(Cell *cell1, Cell *cell2)
{
    if (cell1->x_coordinate == cell2->x_coordinate
        && cell1->y_coordinate == cell2->y_coordinate) {
        return 0;
    } else {
        return 1;
    }
}

static short cmp_cells_values(Cell *cell1, Cell *cell2)
{
    if (cell1->value == cell2->value) {
        return 0;
    } else {
        return 1;
    }
}

static bool **init_visited()
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    bool **visited = malloc(sizeof(bool*) * g_game_settings.field_width);

    for (int i = 0; i < g_game_settings.field_width; i++) {
        visited[i] = malloc(sizeof(bool) * g_game_settings.field_height);

        for (int j = 0; j < g_game_settings.field_height; j++) {
            visited[i][j] = false;
        }
    }

    return visited;
}

static void dfs_search_step(short fill_val, int x, int y, DFSState *dfs_state)
{
    if (!dfs_state->visited[x][y]) {
        dfs_state->visited[x][y] = true;

        if (dfs_state->field[x][y] == fill_val) {
            Cell *cell = malloc(sizeof(Cell));
            cell->value = fill_val;
            cell->x_coordinate = x;
            cell->y_coordinate = y;
            DL_APPEND(dfs_state->matched_list, cell);
            int **adjacents = get_adjacent_coordinates(x, y);
            int i;
            for (i = 0; i < 4 && adjacents[i] != NULL; i++) {
                dfs_search_step(fill_val, adjacents[i][0], adjacents[i][1], dfs_state);
            }
            free_ptr_array((void**) adjacents, i);
        } else {
            Cell *cell = malloc(sizeof(Cell));
            cell->value = dfs_state->field[x][y];
            cell->x_coordinate = x;
            cell->y_coordinate = y;

            Cell *out = NULL;
            DL_SEARCH(dfs_state->reached_list, out, cell, cmp_cells_values);
            if (out == NULL) {
                DL_APPEND(dfs_state->reached_list, cell);
            }
        }
    }
}

static void delete_list(Cell *list)
{
    Cell *cell;
    Cell *tmp;
    DL_FOREACH_SAFE(list, cell, tmp) {
        DL_DELETE(list, cell);
        free(cell);
    }
}

static void dfs_flood_fill(int x, int y, DFSState *dfs_state)
{
    int fill_val = dfs_state->field[x][y];
    dfs_search_step(fill_val, x, y, dfs_state);
}

static void dfs_refresh(DFSState *dfs_state) {
    free(dfs_state->visited);
    dfs_state->visited = init_visited();

    delete_list(dfs_state->reached_list);
    dfs_state->reached_list = NULL;

    /*
     * Don't delete matched list elements yet, as they are still referenced
     * in already_counted_list
     */
    dfs_state->matched_list = NULL;
}

void count_score(float *score_player1, float *score_player2, short **field)
{
    Cell *already_counted_list = NULL;
    *score_player1 = 0;
    *score_player2 = 0;

    DFSState dfs_state;
    dfs_state.field = copy_field(field);
    dfs_state.visited = init_visited();
    dfs_state.matched_list = NULL;
    dfs_state.reached_list = NULL;
    for (int j = 0; j < g_game_settings.field_height; j++) {
        for (int i = 0; i < g_game_settings.field_width; i++) {
            Cell cell;
            cell.x_coordinate = i;
            cell.y_coordinate = j;
            cell.value = field[i][j];

            Cell *out = NULL;
            DL_SEARCH(already_counted_list, out, &cell, cmp_cells_coordinates);
            if (cell.value == 0 && out == NULL) {
                assert(out == NULL);
                dfs_flood_fill(cell.x_coordinate, cell.y_coordinate, &dfs_state);

                DL_CONCAT(already_counted_list, dfs_state.matched_list);
                int count = 0;
                DL_COUNT(dfs_state.reached_list, out, count);
                if (count == 1) {
                    count = 0;
                    DL_COUNT(dfs_state.matched_list, out, count);
                    if (dfs_state.reached_list[0].value == 1) {
                        *score_player1 += count;
                    } else if (dfs_state.reached_list[0].value == 2) {
                        *score_player2 += count;
                    }
                }
                dfs_refresh(&dfs_state);
            } else if (cell.value == 1) {
                *score_player1 += 1;
            } else if (cell.value == 2) {
                *score_player2 += 1;
            }
        }
    }
    delete_list(dfs_state.reached_list);
    delete_list(already_counted_list);
    delete_list(dfs_state.matched_list);
    free_ptr_array((void**) dfs_state.visited, g_game_settings.field_width);

    *score_player2 += KOMI;
}