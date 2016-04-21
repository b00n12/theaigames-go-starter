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

#ifndef MOVE_H
#define MOVE_H

#include <stdbool.h>

typedef struct Move {
    bool is_pass;
    unsigned int x_pos;
    unsigned int y_pos;

    /*
     * Needed for usage of utlist
     */
    struct Move *prev;
    struct Move *next;
} Move;

#endif