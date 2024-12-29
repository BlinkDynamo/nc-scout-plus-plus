#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

/*
 * @brief Validates if the correct number of arguments are provided.
 * @param argc The number of command-line arguments.
 * @return bool Returns true if the number of arguments is valid (2 or 3).
 */
bool validate_arg_count(int argc);

/*
 * @brief Validates if the specified directory exists.
 * @param dir_path The directory path to check.
 * @return bool Returns true if the directory exists.
 */
bool validate_directory_exists(const char *dir_path);

/*
 * @brief Validates if the specified configuration file exists.
 * @param config_path The path to the configuration file.
 * @return bool Returns true if the configuration file exists.
 */
bool validate_config_file_exists(const char *config_path);

#endif // VALIDATION_H