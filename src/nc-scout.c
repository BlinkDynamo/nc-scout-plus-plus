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
#include <regex.h>

#include "nc-scout.h"
#include "validate.h"
#include "naming.h"
#include "search.h"
#include "version.h"
#include "help.h"

// Flag externs.
bool full_path_flag = false;
bool matches_flag   = false;
bool strict_flag    = false;
bool recursive_flag = false;

// Expects a supported naming convention and existing directory in argv[1] and argv[2] respectively. 
// If given these arguments, after asserting they are valid, recursively search the directory for 
// regex matches to the convention and print them to stdout.
int subc_exec_search(int argc, char *argv[])
{
    (void)argc; // Suppress unused argument warning.
    const char *arg_naming_convention = argv[1];
    const char *arg_target_dirname = argv[2]; 
   
    // Set to Conventions[i].regex if arg_naming_convention is valid, otherwise it remains NULL.
    char *search_expression;
    // Set by naming_compile_regex() after search_expression is known to be set.
    regex_t search_regex;

    if ((validate_target_dirname_exists(arg_target_dirname)) &&
        (naming_set_expression(arg_naming_convention, &search_expression)) &&
        (naming_compile_regex(&search_regex, search_expression))) {
        
        search_directory(arg_target_dirname, search_regex);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int print_help(int argc)
{
    // Input must be either nc-scout -h or nc-scout --help exactly to recieve the general nc-scout help.
    // Otherwise, they likely intend to recieve help regarding a command directly preceding -h or --help.
    if (argc == 2) {
        printf("%s", HELP_MESSAGE);
        return EXIT_SUCCESS;
    } else {
        printf("Incorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
        return EXIT_FAILURE;
    }
}

int print_version(int argc)
{   
    // Input must be either nc-scout -v or nc-scout --version exactly.
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
            {"strict", no_argument, 0, 's'},
            {"recursive", no_argument, 0, 'R'},
            {0, 0, 0, 0}
        };
        
        int option_index = 0;
        current_opt = getopt_long (argc, argv, "vhfmsR", long_options, &option_index);
        // Break if at the end of the options.
        if (current_opt == -1)
            break;

        switch (current_opt) {
        // These options return a function
        case '?':
            // Check for unknown options first. Exit program if found.
            return EXIT_FAILURE;

        case 'v':
            return print_version(argc);

        case 'h':
            return print_help(argc);
        // These options set flags
        case 'f':
            full_path_flag = true;
            break;

        case 'm':
            matches_flag = true;
            break;

        case 's':
            strict_flag = true;
            break;

        case 'R':
            recursive_flag = true;
            break;

        default:
            abort();
        }
    }
    
    struct Subcommand 
    {   
        char *name;
        int (*execute)(int argc, char *argv[]); // Pointer to the function that executes a subcommand.
    };

    struct Subcommand Subcommands[] = 
    {
        {"search", subc_exec_search}
    };

    int n_subcommands = sizeof(Subcommands) / sizeof(Subcommands[0]);
    
    // Check for no arguments given.
    if ((argc - optind) < 1) { 
        printf("Incorrect usage.\nDo `nc-scout --help` for more information about usage.\n");
        return EXIT_FAILURE;
    }

     // For the remaining command line arguments (non-options), check if the first of them is a subcommand, followed
     // by a correct number of arguments for that subcommand's n_required_args. If so, return the appropriate subc_exec
     // function with these followed arguments.
    if (optind < argc) {
        for (int i = 0; i < n_subcommands; i++) {
            int non_option_argc = argc - optind;
            if (strcmp(argv[optind], Subcommands[i].name) == 0) {
                return Subcommands[i].execute(non_option_argc, &argv[optind]);
            }
        }
        // If this point is reached, no valid subcommand was found.
        printf("Error: Unknown subcommand `%s`.\n", argv[optind]);
        printf("Do `nc-scout --help` for usage information.\n");
        return EXIT_FAILURE;
    }
}
