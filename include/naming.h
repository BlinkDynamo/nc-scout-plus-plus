/*
 * @author	Josh Hayden
 * @file	naming.h
 * @license	GPL-3.0
 */

#ifndef NAMING_H
#define NAMING_H

#include <stdbool.h>

bool naming_match_regex(const char *pattern, const char *file_name);

#endif
