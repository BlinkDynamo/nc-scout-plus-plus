/*
 * @file	nc-scout.c
 * @author	Josh Hayden
 * @brief	An extremely simple naming convention checker.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validate.h"
#include "naming.h"
#include "search.h"

int main(int argc, char *argv[]) 
{
	const char *arg_naming_convention = argv[1];
	const char *arg_target_dirname = argv[2]; 

	int matches = 0;

	if (
	(validate_arg_count(argc)) &&
	(validate_target_dirname_exists(arg_target_dirname)) &&
	(validate_arg_naming_convention(arg_naming_convention))
	) {
		search_directory(arg_target_dirname, arg_naming_convention, &matches);
		printf("%d\n", matches);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}
