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

const char *abs_initial_search_path;
bool initial_search = true;

// Returns the difference of an absolute initial path and an absolute current path.
const char *get_relative_path(const char *abs_initial_path, const char *abs_current_path) {
    while (*abs_initial_path && *abs_current_path && *abs_initial_path == *abs_current_path) {
        abs_initial_path++;
        abs_current_path++;
    }
    return abs_current_path;
}

// Compares a d_name to a regular expression. Depends on matches_flag to be accessible.
void process_current_file(struct dirent *dp, char abs_current_file_path[PATH_MAX], regex_t regex)
{ 
    // Terminate this newly created absolute initial search path with a "/" to allow get_relative path
    // to remove it later.

    if (matches_flag == true) {
        if (naming_match_regex(regex, dp->d_name)) {
            // Matches with full path.  
            if (full_path_flag == true) 
                printf("%s\n", abs_current_file_path); 
            // Matches with relative path.
            else if (full_path_flag == false)
                printf("%s\n", get_relative_path(abs_initial_search_path, abs_current_file_path));
        }
    } 
    else {
        if (!naming_match_regex(regex, dp->d_name)) {
            // Non-matches with full path. 
            if (full_path_flag == true)
                printf("%s\n", abs_current_file_path);
            // Non-matches with relative path.
            else if (full_path_flag == false)
                printf("%s\n", get_relative_path(abs_initial_search_path, abs_current_file_path));
        }
    }
}   

// Search dir_path for files and directories that match or do not match the regular expression
// 'search_expression', depending on the state of matches_flag. This will be done recursively
// if recursive_flag is true.
void search_directory(const char *search_path, regex_t regex)
{
    const char *abs_search_path = canonicalize_file_name(search_path);
    
    // If this is the first time search_directory has been called, save the absolute initial search path.
    if (initial_search == true) {
        abs_initial_search_path = strcat(abs_search_path, "/");
        initial_search = false;
    }

    // dir_path is known to exist at this point, but opendir() can still fail from permissions.
    DIR *current_dir = opendir(abs_search_path);
    if (current_dir == NULL) {
        printf("Error: cannot access %s due to Error %d (%s).\n", abs_search_path, errno, strerror(errno));
        return;
    }
    
    struct dirent *dp;
    while ((dp = readdir(current_dir)) != NULL) {
        // Skip current and parent entries.
        if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
            continue;

        // Create a variable 'current_file' with the supplied path and dp->d_name combined.
        // If the supplied path ends in a '/', remove it before concatenating path and dp-d_name
        char abs_current_file_path[PATH_MAX];
        snprintf(abs_current_file_path, sizeof(abs_current_file_path), "%s/%s", abs_search_path, dp->d_name);

        // Determine file type and perform actions accordingly.
        struct stat statbuf;
        if (lstat(abs_current_file_path, &statbuf) != 0) {
            printf("Error: lstat failed due to Error %d (%s).\n", errno, strerror(errno));
            return;
        }

        // current_file is a directory.
        if (S_ISDIR(statbuf.st_mode)) {
            process_current_file(dp, abs_current_file_path, regex);
            // Recurse each subdirectory if --recursive is set.
            if (recursive_flag)
                search_directory(abs_current_file_path, regex);
        // current_file is a regular file.
        } else if (S_ISREG(statbuf.st_mode)) { 
            process_current_file(dp, abs_current_file_path, regex);
        }
    }
    closedir(current_dir);
}
