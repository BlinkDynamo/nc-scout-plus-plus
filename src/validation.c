/*------------------------------------------------------------
 * @file		validation.c
 * @author	Josh Hayden
 * @brief		Validation of the user input.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <unistd.h>
#include "validation.h"

bool validate_arg_count(int argc) {
    if (argc <= 1 || argc > 3) {
        fprintf(stderr, "Usage: [config_file] directory\n");
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

bool validate_config_file_exists(const char *config_path) {
    if (access(config_path, F_OK) != 0) {
        fprintf(stderr, "Error: Configuration file '%s' does not exist.\n", config_path);
        return false;
    }
    return true;
}