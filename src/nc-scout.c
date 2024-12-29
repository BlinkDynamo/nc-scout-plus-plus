/*------------------------------------------------------------
 * @file		nc-scout.c
 * @author	Josh Hayden
 * @brief		A naming convention checker program.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "validation.h"

int main(int argc, char *argv[]) {
    const char *defaultConfigLocation = "/home/blink/.config/nc-scout/nc-scout.conf";
    const char *arg_Directory = NULL;
    const char *arg_ConfigFile = NULL;

    // Validate argument count
    if (!validate_arg_count(argc)) {
        return EXIT_FAILURE;
    }

    // 1 argument: Check directory and default config
    if (argc == 2) {
        if (!validate_directory_exists(argv[1])) {
            return EXIT_FAILURE;
        }
        if (!validate_config_file_exists(defaultConfigLocation)) {
            return EXIT_FAILURE;
        }
        arg_Directory = argv[1];
        arg_ConfigFile = defaultConfigLocation;
    }

    // 2 arguments: Check both directory and config
    if (argc == 3) {
        if (!validate_directory_exists(argv[2])) {
            return EXIT_FAILURE;
        }
        if (!validate_config_file_exists(argv[1])) {
            return EXIT_FAILURE;
        }
        arg_Directory = argv[2];
        arg_ConfigFile = argv[1];
    }

    // Directory listing and validation (stub for now)
    DIR *dir = opendir(arg_Directory);
    if (dir) {
        struct dirent *dp;
        while ((dp = readdir(dir)) != NULL) {
            printf("%s\n", dp->d_name);
        }
        closedir(dir);
    } else {
        fprintf(stderr, "Error: Could not open directory '%s'.\n", arg_Directory);
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}