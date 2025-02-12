/**********************************************************************************************
*
*   search - nc-scout search handling, operation, and output. 
*
*   LICENSE: zlib/libpng 
*
*   Copyright (c) 2024-2025 Josh Hayden (@BlinkDynamo)
*
*   This software is provided ‘as-is’, without any express or implied
*   warranty. In no event will the authors be held liable for any damages
*   arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute it
*   freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not
*   claim that you wrote the original software. If you use this software
*   in a product, an acknowledgment in the product documentation would be
*   appreciated but is not required.
*
*   2. Altered source versions must be plainly marked as such, and must not be
*   misrepresented as being the original software.
*
*   3. This notice may not be removed or altered from any source
*   distribution. 
*
*********************************************************************************************/

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

/*
 * Each search expression is made up of at least 3 and at most 5 sections:
 *
 *   1. START_OF_STRING: Mandatory. Keeps a critical part of the regular expression separate.
 *  
 *   2. EXPR_BEGIN_DOTFILE: On by default. A non-capturing group at the beginning to allow for dotfiles (ie .local/):
 *          
 *          ^\\.?
 *
 *   3. EXPR_BODY_: Mandatory. A middle body that enforces the required naming convention:
 *          
 *          [a-z0-9]+   'flatcase' in this example.
 *
 *   4. EXPR_END_EXTENTION: On by default. A non-capturing group at the end that allows for the file extention (ie .txt):
 *          
 *          (\\.[a-z0-9]+?)
 *   
 *   5. END_OF_STRING: Mandatory. Keeps a critical part of the regular expression separate.
 */
#define START_OF_STRING         "^"

#define EXPR_BEGIN_DOTFILE      "\\.?"

#define EXPR_BODY_FLATCASE       "[a-z0-9]+"
#define EXPR_BODY_CAMELCASE      "^([a-z]+)([A-Z][a-z0-9]+)+"
#define EXPR_BODY_PASCALCASE     "^([A-Z][a-z0-9]+)+"
#define EXPR_BODY_SNAKECASE      "^[a-z0-9]+(_[a-z0-9]+)+"
#define EXPR_BODY_CONSTANTCASE   "^[A-Z0-9]+(_[A-Z0-9]+)+"
#define EXPR_BODY_KEBABCASE      "^[a-z0-8]+(-[a-z0-9]+)+"
#define EXPR_BODY_COBOLCASE      "^[A-Z0-9]+(-[A-Z0-9]+)+"

#define EXPR_END_EXTENTION      "(\\.[A-Z0-9]+)?"

#define END_OF_STRING           "$"

/*
 * Compiles a regular expression given a naming convention. Returns a dynamically allocated string of
 * combined regular expressions.
 */
char *compile_search_expression(const char *arg_naming_convention)
{
    char *search_expr_body;
    
    if (strcmp("flatcase", arg_naming_convention) == 0)             search_expr_body = EXPR_BODY_FLATCASE;
    else if (strcmp("camelcase", arg_naming_convention) == 0)       search_expr_body = EXPR_BODY_CAMELCASE;
    else if (strcmp("pascalcase", arg_naming_convention) == 0)      search_expr_body = EXPR_BODY_PASCALCASE;
    else if (strcmp("snakecase", arg_naming_convention) == 0)       search_expr_body = EXPR_BODY_SNAKECASE;
    else if (strcmp("constantcase", arg_naming_convention) == 0)    search_expr_body = EXPR_BODY_CONSTANTCASE;
    else if (strcmp("kebabcase", arg_naming_convention) == 0)       search_expr_body = EXPR_BODY_KEBABCASE;
    else if (strcmp("cobolcase", arg_naming_convention) == 0)       search_expr_body = EXPR_BODY_COBOLCASE;

    size_t total_length = strlen(START_OF_STRING) + strlen(EXPR_BEGIN_DOTFILE) + strlen(search_expr_body) + strlen(EXPR_END_EXTENTION) + strlen(END_OF_STRING);
    char *search_expr_final = (char *)malloc(total_length); 

    snprintf(search_expr_final, total_length, "%s%s%s%s%s", START_OF_STRING, EXPR_BEGIN_DOTFILE, search_expr_body, EXPR_END_EXTENTION, END_OF_STRING);
    
    /* Returns a pointer to allocated memory! */
    return search_expr_final;
} 


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
    char *search_expression = compile_search_expression(arg_naming_convention);  

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
    free(search_expression);
}
