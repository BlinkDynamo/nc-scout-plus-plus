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
#include <errno.h>
#include <sys/stat.h>
#include <linux/limits.h>

#include "validate.h"
#include "naming.h"
#include "search.h"
#include "nc-scout.h"

/* Prints a d_name formatted according to full_path_flag. Depends on full_path_flag to be accessible. */
void print_filename(struct dirent *dp, char full_path[PATH_MAX])
{
    if (full_path_flag == true)
        printf("%s\n", full_path);

    else if (full_path_flag == false)
        printf("%s\n", dp->d_name);
}

/* Compares a d_name to a regular expression. Depends on matches_flag to be accessible. */
void process_current_file(struct dirent *dp, char full_path[PATH_MAX], regex_t regex)
{
    if (matches_flag == true) {
        if (naming_match_regex(regex, dp->d_name))
            print_filename(dp, full_path);
    } 
    else if (matches_flag == false) {
        if (!naming_match_regex(regex, dp->d_name))
            print_filename(dp, full_path);
    }   
}

/*
 * Search dir_path for files and directories that match or do not match the regular expression
 * 'search_expression', depending on the state of matches_flag. This will be done recursively
 * if recursive_flag is true.
 */
void search_directory(const char *dir_path, regex_t regex)
{
    /* dir_path is known to exist at this point, but opendir() can still fail from permissions. */
    DIR *dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Error: cannot access %s due to Error %d (%s).\n", 
                dir_path,
                errno,
                strerror(errno));
        return;
    }
    struct dirent *dp;

    while ((dp = readdir(dir)) != NULL) {
        /* Skip current and parent entries. */
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        /* Strip an extra '/' if the dir_path provided ended in a '/'. */
        char full_path[PATH_MAX];
        if (dir_path[strlen(dir_path) - 1] == '/') {
            snprintf(full_path, sizeof(full_path), "%s%s", dir_path, dp->d_name);
        } else {
            snprintf(full_path, sizeof(full_path), "%s/%s", dir_path, dp->d_name);
        }

        struct stat statbuf;
        if (lstat(full_path, &statbuf) == 0) {
            /* Directory. */
            if (S_ISDIR(statbuf.st_mode)) {
                process_current_file(dp, full_path, regex);
                /* Recurse each subdirectory if --recursive is set. */
                if (recursive_flag)
                    search_directory(full_path, regex);
            /* Regular file. */
            } else if (S_ISREG(statbuf.st_mode)) { 
                process_current_file(dp, full_path, regex);
            }
        }
    }
    closedir(dir);
}
