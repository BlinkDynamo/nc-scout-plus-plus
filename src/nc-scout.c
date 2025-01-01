/*------------------------------------------------------------
 * @file	nc-scout.c
 * @author	Josh Hayden
 * @brief	A naming convention checker program.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include "validation.h"
#include "naming.h"
#include "config.h"

int main(int argc, char *argv[]) {
    const char *configFile = "/home/blink/Repositories/nc-scout/config/nc-scout.conf";
    const char *arg_Directory = NULL;

    /* -------------------- Input Validation -------------------- */
    /* Validate argument count */
    if (!validate_arg_count(argc)) {
        return EXIT_FAILURE;
    }

    /* 1 argument: Check directory and that config exists */
    if (argc == 2) {
        if (!validate_directory_exists(argv[1])) {
            return EXIT_FAILURE;
        }
        if (!validate_config_file_exists(configFile)) {
            return EXIT_FAILURE;
        }
        arg_Directory = argv[1];
    }
    
    /* -------------------- Config Parsing -------------------- */
	/* load the user configuration */
	struct config userConfig = load_config(configFile);

    /* -------------------- Name Check -------------------- */
    /* Do the naming convention search inside dir */
    DIR *dir = opendir(arg_Directory);
    if (dir) {
        struct dirent *dp;
	/* For each filename in dir, check it's naming against the config */
        while ((dp = readdir(dir)) != NULL) {
            printf("%s\n", dp->d_name);
	    check_naming(dp->d_name);
        }
        closedir(dir);
    } else {
        fprintf(stderr, "Error: Could not open directory '%s'.\n", arg_Directory);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS; 
}
