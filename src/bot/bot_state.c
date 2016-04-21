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

#include "bot_state.h"

Settings g_game_settings = {
    .my_bot_id = INITIALVALUE16,
    .opponent_bot_id = INITIALVALUE16,
    .timebank = INITIALVALUE64,
    .time_per_move = INITIALVALUE64,
    .field_width = INITIALVALUE32,
    .field_height = INITIALVALUE32,
    .max_rounds = INITIALVALUE32
};

BotState g_bot_state;
