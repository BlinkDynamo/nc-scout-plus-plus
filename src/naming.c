/*------------------------------------------------------------
 * @file	naming.c
 * @author	Josh Hayden
 * @brief	Compares a filename and a convention.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "naming.h"

bool naming_check_camelcase(const char *file_name, const char *naming_convention) {
	printf("%s\n", file_name);
	return true;
}

bool naming_check_snakecase(const char *file_name, const char *naming_convention) {
	printf("%s\n", file_name);
	return true;
}

bool naming_check_kebabcase(const char *file_name, const char *naming_convention) {
	printf("%s\n", file_name);
	return true;
}
