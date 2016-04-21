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

#include "field.h"
#include "../util.h"
#include "../utlist.h"

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

short **create_new_field()
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    short **field = malloc(sizeof(short*) * g_game_settings.field_width);

    for (int i = 0; i < g_game_settings.field_width; i++) {
        for (int j = 0; j < g_game_settings.field_height; j++) {
            field[i] = malloc(sizeof(short) * g_game_settings.field_height);
        }
    }

    clear_field(field);
    return field;
}

void destroy_field(short **field)
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    for (int i = 0; i < g_game_settings.field_width; i++) {
        free(field[i]);
    }
    free(field);
}

short **parse_field(char *str)
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    short **field = create_new_field();

    char **splitted_str = str_split(str, ",;");
    int counter = 0;
    for (int j = 0; j < g_game_settings.field_height; j++) {
        for (int i = 0; i < g_game_settings.field_width; i++) {
            field[i][j] = atoi(splitted_str[counter]);
            free(splitted_str[counter]);
            counter++;
        }
    }
    free(splitted_str);
    return field;
}

void clear_field(short **field)
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    for (int i = 0; i < g_game_settings.field_width; i++) {
        for (int j = 0; j < g_game_settings.field_height; j++) {
            field[i][j] = 0;
        }
    }
}

static int flood(short **field, bool **mark, int x, int y, int player,
                 int stack_counter)
{
    if (x < 0) return 0;
    if (y < 0) return 0;
    if (x >= g_game_settings.field_width) return 0;
    if (y >= g_game_settings.field_height) return 0;

    if (mark[x][y]) return 0;

    if (field[x][y] != player) {
        if (field[x][y] == 0) {
            return 1;
        } else {
            return 0;
        }
    }

    mark[x][y] = true;

    int neighbor_liberty_count = 0;
    if (stack_counter < g_game_settings.field_width * g_game_settings.field_height) {
        neighbor_liberty_count += flood(field, mark, x - 1, y, player,
                                        stack_counter + 1);
        neighbor_liberty_count += flood(field, mark, x + 1, y, player,
                                        stack_counter + 1);
        neighbor_liberty_count += flood(field, mark, x, y - 1, player,
                                        stack_counter + 1);
        neighbor_liberty_count += flood(field, mark, x, y + 1, player,
                                        stack_counter + 1);
    }

    return neighbor_liberty_count;
}

static bool is_suicide_move(short **field, int x, int y)
{
    assert(field[x][y] == 0);

    bool **mark = malloc(sizeof(bool) * g_game_settings.field_height
                            * g_game_settings.field_width);
    for (int i = 0; i < g_game_settings.field_width; i++) {
        for (int j = 0; j < g_game_settings.field_height; j++) {
            mark[i][j] = false;
        }
    }

    for (int j = 0; j < g_game_settings.field_height; j++) {
        for (int i = 0; i < g_game_settings.field_width; i++) {
            mark[i][j] = false;
        }
    }

    field[x][y] = g_game_settings.my_bot_id;
    int liberties = flood(field, mark, x, y, g_game_settings.my_bot_id, 0);
    field[x][y] = 0;

    for (int i = 0; i < g_game_settings.field_width; i++) {
        free(mark[i]);
    }
    free(mark);
    return liberties <= 0;
}

static bool is_empty_point(short **field, int x, int y)
{
    return field[x][y] == 0;
}

Move *get_available_moves(short **field)
{
    assert(g_game_settings.field_width != INITIALVALUE64
            && g_game_settings.field_height != INITIALVALUE64);

    Move *list_head = NULL;

    for (int j = 0; j < g_game_settings.field_height; j++) {
        for (int i = 0; i < g_game_settings.field_width; i++) {
            if (is_empty_point(field, i, j) && !is_suicide_move(field, i, j)) {
                Move *move = malloc(sizeof(Move));
                move->x_pos = i;
                move->y_pos = j;
                DL_APPEND(list_head, move);
            }
        }
    }

    return list_head;
}

const char *field_to_str(short **field)
{
    int len = g_game_settings.field_width * g_game_settings.field_height
                * sizeof(char) * 2 + g_game_settings.field_height * sizeof(char)
                    + sizeof(char);
    char *str = malloc(len);
    int count = 0;
    for (int j = 0; j < g_game_settings.field_height; j++)
    {
        for (int i = 0; i < g_game_settings.field_width; i++)
        {
            assert(field[i][j] >= 0 && field[i][j] < 3);
            sprintf(str + count, "%d", field[i][j]);
            count++;
            str[count] = ' ';
            count++;
        }
        str[count] = '\n';
        count++;
    }
    str[count] = '\0';

    return str;
}
