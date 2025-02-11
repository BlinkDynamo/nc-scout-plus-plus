/**********************************************************************************************
*
*   nc-scout - A simple naming convention tool.
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
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "nc-scout.h"
#include "validate.h"
#include "naming.h"
#include "search.h"
#include "version.h"

/* flag externs */
bool full_path_flag = false;
bool matches_flag = false;

/*
 * Expects a supported naming convention and existing directory in argv[1] and argv[2] respectively. If given these arguments, 
 * after asserting they are valid, recursively search the directory for regex matches to the convention and print them to stdout.
 */
int subc_exec_search(int argc, char *argv[])
{
	const char *arg_naming_convention = argv[1];
	const char *arg_target_dirname = argv[2]; 

	if ((validate_target_dirname_exists(arg_target_dirname)) &&
	   (validate_arg_naming_convention(arg_naming_convention))) {
		search_directory(arg_target_dirname, arg_naming_convention);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int subc_exec_help(int argc, char *argv[])
{
	char *message = 
	"Usage: nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]\n"
	"\n"
	"Options:\n"
	"  -h, --help       Show this help message\n"
	"  -v, --version    Show the program version\n"
	" -f, --full-path   Display files as a full path\n"
	" -m, --matches     Display files that match the provided naming convention\n"
	"\n"
	"Commands:\n"
	"  search           Search for filenames that do not match a naming convention in a directory\n"
	"\n"
	"Conventions:\n"
	"  camelcase        exampleFileName.txt\n"
	"  snakecase        example_file_name.txt\n"
	"  kebabcase        example-file-name.txt\n"
	"";
	/* Input must be either nc-scout -h or nc-scout --help exactly to recieve the general nc-scout help. 
	 * Otherwise, they likely intend to recieve help regarding a command directly preceding -h or --help. */
	if (argc == 2) {
		printf("%s", message);
		return EXIT_SUCCESS;
	} else {
		printf("Incorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
		return EXIT_FAILURE;
	}
}

int subc_exec_version(int argc, char *argv[])
{	
	/* Input must be either nc-scout -v or nc-scout --version exactly. */
	if (argc == 2) {
		printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
		return EXIT_SUCCESS;
	} else {
		printf("Incorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
		return EXIT_FAILURE;
	}
}

int main(int argc, char *argv[]) 
{	
	int current_opt;
	while (1) {
		static struct option long_options[] = 
		{
			{"version", no_argument, 0, 'v'},
			{"help", no_argument, 0, 'h'},
			{"full-path", no_argument, 0, 'f'},
			{"matches", no_argument, 0, 'm'},
			{0, 0, 0, 0}
		};
		
		int option_index = 0;
		current_opt = getopt_long (argc, argv, "vhfm", long_options, &option_index);
		/* Break if at the end of the options. */
		if (current_opt == -1)
			break;

		switch (current_opt) {
		/* These options return a function */
		case '?':
			/* Check for unknown options first. Exit program if found. */
			return EXIT_FAILURE;

		case 'v':
			return subc_exec_version(argc, argv);

		case 'h':
			return subc_exec_help(argc, argv);
		/* These options set flags */
		case 'f':
			full_path_flag = true;
			break;

		case 'm':
			matches_flag = true;
			break;

		default:
			abort();
		}
	}
	
	struct subcommand 
	{	
		char *name;
		int n_required_args; /* Only sub-arguments to the subcommand. Does not include the subcommand itself. */
		int (*execute)(int argc, char *argv[]); /* Pointer to the function that executes a subcommand. */
	};

	struct subcommand subcommands[] = 
	{
		{"search", 2, subc_exec_search}
	};

	int n_subcommands = sizeof(subcommands) / sizeof(subcommands[0]);
	
	/* Check for no arguments given. */
	if ((argc - optind) < 1) { 
		printf("Incorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
		return EXIT_FAILURE;
	}

	/* For the remaining command line arguments (non-options), check if the first of them is a subcommand, followed
	 * by a correct number of arguments for that subcommand's n_required_args. If so, return the appropriate subc_exec 
	 * function with these followed arguments. */
	if (optind < argc) {
		bool subcommand_valid_match_found = false;
		for (int i = 0; i < n_subcommands; i++) {
			int non_option_argc = argc - optind;
			if ((strcmp(argv[optind], subcommands[i].name) == 0) && 
			    (subcommands[i].n_required_args == (non_option_argc - 1))) /* -1 to account for command itself */
			return subcommands[i].execute(non_option_argc, &argv[optind]);
		}

		if (!subcommand_valid_match_found) {
			printf("Error: Unknown subcommand `%s`.\n", argv[optind]);
			printf("Do `nc-scout --help` for usage information.\n");
			return EXIT_FAILURE;
		}
	}
}
