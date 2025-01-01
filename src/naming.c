/*------------------------------------------------------------
 * @file	naming.c
 * @author	Josh Hayden
 * @brief	The final check of the specified naming convention.
 *----------------------------------------------------------*/

#include <stdio.h>
#include "naming.h"

int check_naming(const char *file_name) {
    printf("From naming.c: %s\n", file_name);
    return 1;
}
