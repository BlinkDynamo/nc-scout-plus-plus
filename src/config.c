/*------------------------------------------------------------
 * @file		config.c
 * @author		Josh Hayden
 * @brief		Parsing and understanding of the configurations.
 *----------------------------------------------------------*/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "config.h"

struct config load_config(const char *pathToConfigFile) {   	
	/* Create a config structure for the user */
	struct config userConfig; 
	
	/* Initialize defaults */
	userConfig.allowSpaces = true;
	
	/* File reading things */
	FILE* ptr_configFile = fopen(pathToConfigFile, "r");
	char * line = NULL;
	size_t len = 0;
	ssize_t read;
	
	if (ptr_configFile == NULL) {
		fprintf(stderr, "Error: File not found when loading config\n");
	}
	while ((read = getline(&line, &len, ptr_configFile)) != -1) {
			if (line[0] == '#') {
					printf("Comment detected in %s", line);
					continue;
			}
			printf("Retrieved line of length %zu: \n", read);
			printf("%s", line);
	}

	/* Cleanup */
	fclose(ptr_configFile);
	if (line) {
		free(line);
	}

	printf("Configuration file loaded.\n");
    return userConfig;
}
