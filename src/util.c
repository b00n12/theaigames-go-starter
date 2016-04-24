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

#include "util.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LENGTH_STR 64

long random_at_most(long max)
{
  unsigned long
    // max <= RAND_MAX < ULONG_MAX, so this is okay.
    num_bins = (unsigned long) max + 1,
    num_rand = (unsigned long) RAND_MAX + 1,
    bin_size = num_rand / num_bins,
    defect   = num_rand % num_bins;

  long x;
  do {
   x = rand();
  }
  // This is carefully written not to overflow
  while (num_rand - defect <= (unsigned long)x);

  // Truncated division is intentional
  return x/bin_size;
}

char *str_replace(char *str, char *to_replace, char *replacement)
{
    char *temp_str, *search_start;
    int len=0;

    search_start = strstr(str, to_replace);
    if(search_start == NULL) {
        return str;
    }

    temp_str = (char*) malloc(strlen(str) * sizeof(char));
    if(temp_str == NULL) {
        return NULL;
    }

    strcpy(temp_str, str);

    len = search_start - str;
    str[len] = '\0';

    strcat(str, replacement);

    len += strlen(to_replace);
    strcat(str, (char*) temp_str + len);

    free(temp_str);

    return str;
}

char **str_split(char *str, char *delimiter)
{
    int str_len = strlen(str) + 1;
    char *tmp = malloc(sizeof(char) * str_len);
    strcpy(tmp, str);
    char *ptr = malloc(sizeof(char));

    ptr = strtok(str, delimiter);
    int count = 0;
    while (ptr != NULL)
    {
        count++;
        ptr = strtok(NULL, delimiter);
    }

    char **splitted = malloc(count * sizeof(char*));
    ptr = strtok(tmp, delimiter);
    count = 0;
    while (ptr != NULL)
    {
        splitted[count] = malloc(sizeof(char) * (strlen(ptr) + 1));
        strcpy(splitted[count], ptr);
        count++;
        ptr = strtok(NULL, delimiter);
    }
    
    free(ptr);

    return splitted;
}

void free_ptr_array(void **ptr_array, int size)
{
    for (int i = 0; i < size; i++) {
        free(ptr_array[i]);
    }
    free(ptr_array);
}
