/*------------------------------------------------------------
 * @file	validate.c
 * @author	Josh Hayden
 * @brief	Validation of the user input.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "validate.h"

bool validate_arg_count(int argc) {
    if (argc != 3) {
        fprintf(stderr, "Usage: nc-scout <convention> <directory>\n");
        fprintf(stderr, "Error: Incorrect number of arguments.\n");
        return false;
    }
    return true;
}

bool validate_directory_exists(const char *dir_path) {
    if (access(dir_path, F_OK) != 0) {
        fprintf(stderr, "Error: Directory '%s' does not exist.\n", dir_path);
        return false;
    }
    return true;
}

