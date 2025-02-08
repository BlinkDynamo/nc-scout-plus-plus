/*
 * @file	nc-scout.c
 * @author	Josh Hayden
 * @license	GPL-3.0
 */

#define _GNU_SOURCE

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

#define EXPR_CAMELCASE "^([a-z]+)([A-Z][a-z0-9]+)+$"
#define EXPR_SNAKECASE "^[a-z0-9]+(_[a-z0-9]+)+$"
#define EXPR_KEBABCASE "^[a-z0-9]+(-[a-z0-9]+)+$"

/* Prints a d_name formatted according to full_path_flag. Depends on full_path_flag to be accessible. */
void print_filename(struct dirent *dp, char full_path[PATH_MAX])
{
	if (full_path_flag == true)
		printf("%s%s\n", full_path, dp->d_name);

	else if (full_path_flag == false)
		printf("%s\n", dp->d_name);
}

/* Compares a d_name to a regular expression. Depends on matches_flag to be accessible. */
void process_current_file(struct dirent *dp, char full_path[PATH_MAX], char *search_expression)
{
	if (matches_flag == true) {
		if (naming_match_regex(search_expression, dp->d_name))
			print_filename(dp, full_path);
	} 
	else if (matches_flag == false) {
		if (!naming_match_regex(search_expression, dp->d_name))
			print_filename(dp, full_path);
	}	
}
	
/* 
 * Moves recursively down dir_path, looking for files and directories that match or do not match the naming 
 * convention `arg_naming_convention`, depending on the state of matches_flag.
 */
void search_directory(const char *dir_path, const char *arg_naming_convention)
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
		if (lstat(full_path, &statbuf) == 0) {
			if (S_ISDIR(statbuf.st_mode)) {
				process_current_file(dp, full_path, search_expression);
				/* Recurse each subdirectory. */
				search_directory(full_path, arg_naming_convention);
			} else if (S_ISREG(statbuf.st_mode)) { 
				process_current_file(dp, full_path, search_expression);
			}
		}
	}
	closedir(dir);
}
