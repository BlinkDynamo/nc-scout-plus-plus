/*------------------------------------------------------------
 * @file	nc-scout.c
 * @author 	J. Hayden
 * @brief	A naming convention checker.
 *----------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  char defaultConfigLocation[] = "$HOME/.config/nc-scout/nc-scout.conf";

  /* Error and usage strings */
  char error_message_Usage[]                 = "Usage: %s <config_file> <directory>\n";
  char error_message_NoDefaultConf[]         = "To omit a configuration file, a default " 
                                               "configuration must exist at $HOME/.config/nc-scout.conf\n";
  char error_message_DirSpecifiedNotExist[]  = "The directory you specified does not exist\n";
  char error_message_ConfSpecifiedNotExist[] = "The configuration file you specified does not exist\n";
  /* ---------- Input Validation ---------- */ 
  
  /* No arguments supplied */
  if (argc == 1) {
    fprintf(stderr, error_message_Usage, argv[0]);
    return EXIT_FAILURE;
  }
  /* 1 argument is supplied */
  else if (argc == 2) {
    /* The directory supplied is not a valid directory */
    if (access(argv[1], F_OK) != 0) {
      fprintf(stderr, strcat(error_message_Usage, error_message_DirSpecifiedNotExist), argv[0]);
      return EXIT_FAILURE;
    }
    /* There is no default configuration file */
    if (access(defaultConfigLocation, F_OK) != 0) {
      fprintf(stderr, strcat(error_message_Usage, error_message_NoDefaultConf), argv[0]);
      return EXIT_FAILURE;
    }
    /* There is a default configuration file and the directory supplied exists */
    else {
      const char *arg_Directory = argv[1];
      const char *arg_ConfigFile = defaultConfigLocation;
    }
  }
  /* 2 arguments are supplied */
  else if (argc == 3) {
    /* The configuration file specfied does not exist */
    if (access(argv[1], F_OK) != 0) {
      
      
  DIR *dir;
  struct dirent *dp;
  char * file_name;
  const char *directory = arg_Directory;

  dir = opendir(directory);

  while ((dp=readdir(dir)) != NULL) {
	  file_name = dp->d_name;
	  printf("%s",file_name);
  }
  closedir(dir); 
  return EXIT_SUCCESS;
}
