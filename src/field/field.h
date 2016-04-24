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

#ifndef FIELD_H
#define FIELD_H

#include "../bot/bot_state.h"
#include "../move/move.h"

short **create_new_field();
void destroy_field(short **field);
short **copy_field(short **field);
short **parse_field(char *str);
void clear_field(short **field);
Move *get_available_moves(short **field);
const char *field_to_str(short **field);
int **get_adjacent_coordinates(int x, int y);

#endif
