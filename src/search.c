/*
 * @file	nc-scout.c
 * @author	Josh Hayden
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "validate.h"
#include "naming.h"
#include "search.h"
#include "nc-scout.h"

#define EXPR_CAMELCASE "^([a-z]+)([A-Z][a-z]+)+$"
#define EXPR_SNAKECASE "^[a-z]+(_[a-z]+)+$"
#define EXPR_KEBABCASE "^[a-z]+(-[a-z]+)+$"

void print_search_results(struct dirent *dp, char full_path[PATH_MAX])
{
	if (full_path_flag == 1)
		printf("%s%s\n", full_path, dp->d_name);
	else
		printf("%s\n", dp->d_name);
}

/* 
 * Moves recursively down dir_path, looking for files and directories that match the naming 
 * convention `arg_naming_convention`. Increments `matches` on a successfully matching filename.
 */
void search_directory(const char *dir_path, const char *arg_naming_convention, int *matches)
{
	/* dir_path is known to be valid at this point. */	
	DIR *dir = opendir(dir_path);	

	struct dirent *dp;
	char *search_expression;
	if (strcmp("camelcase", arg_naming_convention) == 0)		search_expression = EXPR_CAMELCASE;
	else if (strcmp("snakecase", arg_naming_convention) == 0)	search_expression = EXPR_SNAKECASE;
	else if (strcmp("kebabcase", arg_naming_convention) == 0)	search_expression = EXPR_KEBABCASE;

	while ((dp = readdir(dir)) != NULL) {
		/* Skip current and parent entries. */
		if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
			continue;

		char full_path[PATH_MAX];
		if (dir_path[strlen(dir_path) - 1] == '/') {
			snprintf(full_path, sizeof(full_path), "%s%s", dir_path, dp->d_name);
		} else {
			snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, dp->d_name);
		}

		struct stat statbuf;
		if (stat(full_path, &statbuf) == 0) {
			if (S_ISDIR(statbuf.st_mode)) {
				/* If it's a directory, check it's name then recurse. */
				if (naming_match_regex(search_expression, dp->d_name)) {
					print_search_results(dp, full_path);	
					(*matches)++;
				} 
				/* Recurse each subdirectory. */
				search_directory(full_path, arg_naming_convention, matches);
			} else if (S_ISREG(statbuf.st_mode)) { 
				if (naming_match_regex(search_expression, dp->d_name)) {
					print_search_results(dp, full_path);	
					(*matches)++;
				} 
			}	
		}
	}
	closedir(dir);
}
