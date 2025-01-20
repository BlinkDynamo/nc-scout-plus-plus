#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

bool validate_arg_count(int argc);

bool validate_directory_exists(const char *dir_path);

#endif // VALIDATION_H
