/*
 * @file	nc-scout.c
 * @author	Josh Hayden
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "validate.h"
#include "naming.h"
#include "search.h"
#include "version.h"

static int full_path_flag;

/* 
 * Expects a supported naming convention and existing directory in argv[1] and argv[2] respectively. If given these arguments, 
 * after asserting they are valid, recursively search the directory for regex matches to the convention and print them to stdout.
 */ 
int subc_exec_search(int argc, char *argv[])
{	
	const char *arg_naming_convention = argv[1];
	const char *arg_target_dirname = argv[2]; 

	int matches = 0;
	
	if ((validate_arg_count(argc)) &&
	   (validate_target_dirname_exists(arg_target_dirname)) &&
	   (validate_arg_naming_convention(arg_naming_convention))) {
		search_directory(arg_target_dirname, arg_naming_convention, &matches);
		printf("%d\n", matches);
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}

int subc_exec_help()
{
	char *message = 
	"Usage: nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]\n"
	"\n"
	"Options:\n"
	"  -h, --help       Show this help message\n"
	"  -v, --version    Show the program version\n"
	" -f, --full-path   Display matches as a full path\n"
	"\n"
	"Commands:\n"
	"  search           Search for matches to a naming convention in a directory\n"
	"\n"
	"Conventions:\n"
	"  camelcase        exampleFileName.txt\n"
        "  snakecase        example_file_name.txt\n"
	"  kebabcase        example-file-name.txt\n"	
	"";

	printf("%s", message);
	return EXIT_SUCCESS;
}

int subc_exec_version()
{	
	printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
	return EXIT_SUCCESS;
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
			{0, 0, 0, 0}
		};
		
		int option_index = 0;
		current_opt = getopt_long (argc, argv, "vhf", long_options, &option_index);
		/* Break if at the end of the options. */
		if (current_opt == -1)
			break;

		switch (current_opt) {
		case '?':
			/* Check for unknown options first. Exit program if found. */
			return EXIT_FAILURE;
		case 'v':
			return subc_exec_version(argc, argv);

		case 'h':
			return subc_exec_help(argc, argv);

		case 'f':
			full_path_flag = 1;	
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
		printf("\nIncorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
		return EXIT_FAILURE;
	}

	/* For the remaining command line arguments (non-options), check if the first of them is a subcommand.
	 * If so, return the appropriate subc_exec function with these remaining arguments. */
	if (optind < argc) {
		bool subcommand_valid_match_found = false;

		for (int i = 0; i < n_subcommands; i++) {
			if (strcmp(argv[optind], subcommands[i].name) == 0) 
			return subcommands[i].execute(argc - optind, &argv[optind]);
		}

		if (!subcommand_valid_match_found) {
			printf("Error: Unknown subcommand `%s`.\n", argv[optind]);
			printf("Do `nc-scout --help` for usage information.\n");
			return EXIT_FAILURE;
		}
	}
}
