/*
 * @file	naming.c
 * @author	Josh Hayden
 * @license	GPL-3.0
 */

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
