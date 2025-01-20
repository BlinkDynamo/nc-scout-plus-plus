#ifndef NAMING_H
#define NAMING_H

#include <stdbool.h>

bool naming_check_camelcase(const char *file_name, const char *naming_convention);

bool naming_check_snakecase(const char *file_name, const char *naming_convention);

bool naming_check_kebabcase(const char *file_name, const char *naming_convention);

#endif
