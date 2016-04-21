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

#ifndef BOT_STATE_H
#define BOT_STATE_H

#include "../move/move.h"

#define INITIALVALUE16 0xFFFF
#define INITIALVALUE32 0xFFFFFFFF
#define INITIALVALUE64 0xFFFFFFFFFFFFFFFF

typedef struct Settings {
    unsigned short my_bot_id;
    unsigned short opponent_bot_id;
    unsigned long timebank;
    unsigned long time_per_move;
    unsigned int field_width;
    unsigned int field_height;
    unsigned int max_rounds;
} Settings;

extern Settings g_game_settings;

typedef struct BotState {
    unsigned int round;
    short **field;
    unsigned int tot_num_moves;
    unsigned int my_points;
    unsigned int opponent_points;
    Move my_last_move;
    Move opponent_last_move;
} BotState;

extern BotState g_bot_state;

#endif
