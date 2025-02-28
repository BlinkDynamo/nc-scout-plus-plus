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

#include "search.h"
#include "version.h"
#include "help.h"

int print_help(int argc)
{
    // Input must be either nc-scout -h or nc-scout --help exactly.
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
        static struct option long_options_builtins[] = 
        {
            {"version", no_argument, 0, 'v'},
            {"help", no_argument, 0, 'h'},
            {0, 0, 0, 0}
        };
        
        int option_index = 0;
        current_opt = getopt_long (argc, argv, "+vh", long_options_builtins, &option_index);
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
