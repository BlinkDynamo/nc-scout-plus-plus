/*------------------------------------------------------------
 * @file	nc-scout.c
 * @author	Josh Hayden
 * @brief	An extremely simple naming convention checker.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "validate.h"
#include "naming.h"

int main(int argc, char *argv[]) {
   
	const char *arg_Convention = argv[1];	
	const char *arg_Directory = argv[2];
	
	int matches = 0;
    
	if (
		(validate_arg_count(argc)) && 
		(validate_directory_exists(arg_Directory))
	   )
	{
		/* Do the naming convention search inside dir */
		DIR *dir = opendir(arg_Directory);
		if (dir) {
			struct dirent *dp;
			/* For each filename in dir, check it's naming against the config */
			while ((dp = readdir(dir)) != NULL) {
				if (strcmp("camelcase", arg_Convention) == 0) {
					if (naming_check_camelcase(dp->d_name, arg_Convention)) matches++;
				}
				else if (strcmp("snakecase", arg_Convention) == 0) {
					if (naming_check_snakecase(dp->d_name, arg_Convention)) matches++;
				}
				else if (strcmp("kebabcase", arg_Convention) == 0) {
					if (naming_check_snakecase(dp->d_name, arg_Convention)) matches++;
				}
				else {
					/* Validation outside of validate.c to prevent checking the convention twice */
					fprintf(stderr, "Error: '%s' is not a valid naming convention\n", arg_Convention);
					return EXIT_FAILURE;	
				}
			}
			closedir(dir);
		}
		printf("%d\n", matches);
		return EXIT_SUCCESS; 
    }
	return EXIT_FAILURE;
}
