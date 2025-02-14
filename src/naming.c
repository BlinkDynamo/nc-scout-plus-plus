/**********************************************************************************************
*
*   naming - naming comparison for nc-scout. 
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

#include <stdio.h>
#include <string.h>
#include <regex.h>

#include "naming.h"

/*
 * Each regular expression is made up of three sections:
 *  
 *   1. A non-capturing group at the beginning to allow for dotfiles (ie .local/):
 *      ^\\.?
 *
 *   2. A middle body that enforces the required naming convention:
 *      [a-z0-9]+   'flatcase' in this example.
 *
 *   3. A non-capturing group at the end that allows for the file extention (ie foo.txt):
 *      (\\.[A-Za-z0-9]+?)
 *
 */
#define EXPR_FLATCASE       "^\\.?[a-z0-9]+(\\.[A-Za-z0-9]+)?$"
#define EXPR_CAMELCASE      "^\\.?[a-z]+([A-Z][a-z0-9]+)+(\\.[A-Za-z0-9]+)?$"
#define EXPR_PASCALCASE     "^\\.?([A-Z][a-z0-9]+)+(\\.[A-Za-z0-9]+)?$"
#define EXPR_SNAKECASE      "^\\.?[a-z0-9]+(_[a-z0-9]+)+(\\.[A-Za-z0-9]+)?$"
#define EXPR_CONSTANTCASE   "^\\.?[A-Z0-9]+(_[A-Z0-9]+)+(\\.[A-Za-z0-9]+)?$"
#define EXPR_KEBABCASE      "^\\.?[a-z0-9]+(-[a-z0-9]+)+(\\.[A-Za-z0-9]+)?$"
#define EXPR_COBOLCASE      "^\\.?[A-Z0-9]+(-[A-Z0-9]+)+(\\.[A-Za-z0-9]+)?$"

struct Convention Conventions[] = {
    {"flatcase", EXPR_FLATCASE}, 
    {"camelcase", EXPR_CAMELCASE},
    {"pascalcase", EXPR_PASCALCASE},
    {"snakecase", EXPR_SNAKECASE},
    {"constantcase", EXPR_CONSTANTCASE},
    {"kebabcase", EXPR_KEBABCASE},
    {"cobolcase", EXPR_COBOLCASE},
};

const int n_members_in_Conventions = (sizeof(Conventions) / sizeof(struct Convention));

/* 
 * Attempts to match arg_naming_convention against every Conventions[i].name in Conventions[].
 * If a match is found, dereference the pointer ptr_search_expression points to, set it to
 * Conventions[i].regex, and return true. Otherwise, return false and keep search_expression NULL.
 */
bool naming_set_expression(const char *arg_naming_convention, char **ptr_search_expression)
{
    for (int i = 0; i < n_members_in_Conventions; i++) {
        if (strcmp(arg_naming_convention, Conventions[i].name) == 0) {
            *ptr_search_expression = Conventions[i].regex;
            return true;
        }
    }
    fprintf(stderr, "Error: '%s' is not a valid naming convention\n", arg_naming_convention);
    return false;
}

/* Applies a regex pattern upon a filename, returning true if a match. */
bool naming_match_regex(const char *pattern, const char *file_name)
{
    regex_t regex;
    int regex_return;

    if (regcomp(&regex, pattern, REG_EXTENDED) != 0) {
        fprintf(stderr, "Error: Failed to compile regex.\n");
    }

    regex_return = regexec(&regex, file_name, 0, NULL, 0);
    if (regex_return == 0) {
        return true;
    }

    return false;
}
