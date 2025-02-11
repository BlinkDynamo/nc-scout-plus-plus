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
