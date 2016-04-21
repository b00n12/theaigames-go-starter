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

#include "parser.h"
#include "bot.h"
#include "../field/field.h"
#include "bot_state.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define MAX_LINE_LENGTH 768
//#define DEBUG

void run()
{
    char line[MAX_LINE_LENGTH];
    char part[6][MAX_LINE_LENGTH];
#ifdef DEBUG
    FILE *test_file = fopen("test.txt", "rb");

    if (test_file == NULL) {
        fprintf(stderr, "Error, cannot open test file\n");
        return;
    }
#endif

#ifdef DEBUG
    while (fgets(line, MAX_LINE_LENGTH, test_file) != NULL)
#else
    while (fgets(line, MAX_LINE_LENGTH, stdin) != NULL)
#endif
    {
        if (strlen(line) == 0)
        {
            continue;
        }

        /*
         * Handle "settings" commands
         */
        if (!strncmp(line, "settings ", 9))
        {
            sscanf(&line[9], "%s %s", part[0], part[1]);
            if (!strcmp(part[0], "your_botid"))
            {
                g_game_settings.my_bot_id = atoi(part[1]);
                if (g_game_settings.my_bot_id == 1) {
                    g_game_settings.opponent_bot_id = 2;
                } else {
                    g_game_settings.opponent_bot_id = 1;
                }
            }
            if (!strcmp(part[0], "timebank"))
            {
                g_game_settings.timebank = atol(part[1]);
            }
            if (!strcmp(part[0], "time_per_move"))
            {
                g_game_settings.time_per_move = atol(part[1]);
            }
            if (!strcmp(part[0], "field_width"))
            {
                g_game_settings.field_width = atol(part[1]);
            }
            if (!strcmp(part[0], "field_height"))
            {
                g_game_settings.field_height = atol(part[1]);
            }
            if (!strcmp(part[0], "max_rounds"))
            {
                g_game_settings.max_rounds = atol(part[1]);
            }

        /*
         * Handle "update" commands
         */
        } else if (!strncmp(line, "update ", 7)) {
            sscanf(&line[7], "%s %s %s", part[0], part[1], part[2]);
            if (!strcmp(part[1], "field"))
            {
                char *data = part[2];
                g_bot_state.field = parse_field(data);
            }
            if (!strcmp(part[1], "round"))
            {
                g_bot_state.round = atoi(part[2]);
            }
            if (!strcmp(part[1], "move"))
            {
                g_bot_state.tot_num_moves = atoi(part[2]);
            }
            if (!strcmp(part[1], "points"))
            {
                if ((!strcmp(part[0], "player1") && g_game_settings.my_bot_id == 1)
                    || (!strcmp(part[0], "player2") && g_game_settings.my_bot_id == 2)) {

                    g_bot_state.my_points = atoi(part[2]);
                } else {
                    g_bot_state.opponent_points = atoi(part[2]);
                }
            }
            if (!strcmp(part[1], "last_move"))
            {
                sscanf(&line[7], "%s %s %s %s %s %s", part[0], part[1], part[2],
                            part[3], part[4], part[6]);
                if ((!strcmp(part[0], "player1") && g_game_settings.opponent_bot_id == 1)
                    || (!strcmp(part[0], "player2") && g_game_settings.opponent_bot_id == 2)) {

                    Move last_move;
                    if (!strcmp(part[2], "pass")) {
                        last_move.is_pass = true;
                    } else {
                        last_move.is_pass = false;
                        last_move.x_pos = atoi(part[3]);
                        last_move.y_pos = atoi(part[4]);
                    }
                    g_bot_state.opponent_last_move = last_move;
                }
            }
        } else if (!strncmp(line, "action ", 7)) {
            sscanf(&line[7], "%s %s", part[0], part[1]);
            if (!strcmp(part[0], "move"))
            {
                unsigned long time = atol(part[1]);
                Move move = make_move(time);
                g_bot_state.my_last_move = move;
                if (move.is_pass) {
                    fprintf(stdout, "pass\n");
                } else {
                    fprintf(stdout, "place_move %u, %u\n", move.x_pos, move.y_pos);
                }
                fflush(stdout);
            }
        } else {
            fprintf(stderr, "unknown command\n");
        }
    }
#ifdef DEBUG
    fclose(test_file);
#endif
}