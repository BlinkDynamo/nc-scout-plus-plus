/*------------------------------------------------------------
 * @file	nc-scout.c
 * @author	Josh Hayden
 * @brief	The main search action of the program. Recursively 
 * 			looks through all subdirectories and files of the 
 * 			target for matches to the specified naming convention.	
 *----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

#include "validate.h"
#include "naming.h"
#include "search.h"

#define EXPR_CAMELCASE "^([a-z]+)([A-Z]?[a-z]+)+$"
#define EXPR_SNAKECASE "[_]+"
#define EXPR_KEBABCASE "[-]+"

/* searches through each child directory of dir_path, looking for files and directories
 * that match the naming convention arg_naming_convention. Increments matches on a successfully
 * matching filename */
void search_directory(const char *dir_path, const char *arg_naming_convention, int *matches) {
   	DIR *dir = opendir(dir_path);
	if (dir) {
		struct dirent *dp;	/* directory pointer */
        while ((dp = readdir(dir)) != NULL) {
            
			/* skip current and parent entries */	 
			if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0) continue;	
			
			/* helper function to naming_match_regex(). Will decide what pattern to pass into
			 * naming_match_regex(), pass it in, then increment *matches if it returns true. */	
			void helper_naming_match_regex() {	
				if (strcmp("camelcase", arg_naming_convention) == 0) {
					if (naming_match_regex(EXPR_CAMELCASE, dp->d_name)) {
						printf("%s\n", dp->d_name); 
						(*matches)++;
					}
				} else if (strcmp("snakecase", arg_naming_convention) == 0) {
					if (naming_match_regex(EXPR_SNAKECASE, dp->d_name)) {
						printf("%s\n", dp->d_name); 
						(*matches)++;
					}
				} else if (strcmp("kebabcase", arg_naming_convention) == 0) {
					if (naming_match_regex(EXPR_KEBABCASE, dp->d_name)) {
						printf("%s\n", dp->d_name); 
						(*matches)++;
					}
				} 
			}
			
			/* construct full path and recursively search */ 
            char full_path[PATH_MAX];
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, dp->d_name);

            struct stat statbuf;
            if (stat(full_path, &statbuf) == 0) {
                if (S_ISDIR(statbuf.st_mode)) {
                    /* if it's a directory, check it's name then recurse */
                   	helper_naming_match_regex(); 
                    /* recurse each subdirectory */
                    search_directory(full_path, arg_naming_convention, matches);
				}
			   	else if (S_ISREG(statbuf.st_mode)) helper_naming_match_regex(); 
			}
		}
		closedir(dir);
	}	
}
