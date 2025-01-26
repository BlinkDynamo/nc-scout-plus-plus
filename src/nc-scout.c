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
#include "version.h"

struct subcommand {	
	char *name;
	int n_required_sub_args; /* only sub-arguments to the subcommand. Does not include the subcommand itself. */
	int (*execute)(int argc, char *argv[]); /* pointer to the function that executes a subcommand */
};

int subc_exec_search(int argc, char *argv[])
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

int subc_exec_help(int argc, char *argv[])
{
	char *message = 
	"\n"	
	"Usage: nc-scout [OPTION].. CONVENTION [DIRECTORY]..\n"
	"\n"
	"Options:\n"
	"  -h, --help       Show this help message\n"
	"  -v, --version    Show the program version\n"
	"  search           Search for matches to a naming convention in a directory\n"
	"\n"
	"Conventions:\n"
	"  camelcase\n"
        "  snakecase\n"
	"  kebabcase\n"	
	"\n";

	printf("%s", message);
	return EXIT_SUCCESS;
}

int subc_exec_version(int argc, char *argv[])
{
	if (argc != 0) {
		printf("Unknown command.\nDo `nc-scout help` for more information about usage.\n"); 
		return EXIT_FAILURE;
	}
	printf("%s %s\n", PROGRAM_NAME, PROGRAM_VERSION);
	return EXIT_SUCCESS;
}

int main(int argc, char *argv[]) 
{
	struct subcommand subcommands[] = {
		{"--help", 0, subc_exec_help},
		{"-h", 0, subc_exec_help},	
		{"--version", 0, subc_exec_version},
		{"-v", 0, subc_exec_version},
		{"search", 2, subc_exec_search}
	};

	int n_subcommands = sizeof(subcommands) / sizeof(subcommands[0]);

	if (argc < 2) { 
		printf("\nIncorrect usage.\nDo `nc-scout help` for more information about usage.\n");
		return EXIT_FAILURE;
	}
	
	/* 
	 * compare argv[1] against every subcommand.name in subcommands, and if 
	 * a match is found, execute the matching exec function for that subcommand struct, 
	 * executing it as a return so it shares a return value with main()
	 */ 
	for (int i = 0; i < n_subcommands; i++) {
	    if (strcmp(argv[1], subcommands[i].name) == 0) {
		if (argc - 2 < subcommands[i].n_required_sub_args) {
		    printf("Error: '%s' requires at least %d arguments.\n",
			   subcommands[i].name, subcommands[i].n_required_sub_args);
		    return EXIT_FAILURE;
		}
		return subcommands[i].execute(argc - 1, &argv[1]);
	    }
	}
	printf("Unknown command: %s\n", argv[1]);
	printf("Do nc-scout help for more information about usage.\n");
}

