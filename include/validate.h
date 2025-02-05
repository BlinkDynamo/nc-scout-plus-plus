/*
 * @author	Josh Hayden
 * @file	validate.h
 * @license	GPL-3.0
 */

#ifndef VALIDATION_H
#define VALIDATION_H

#include <stdbool.h>

bool validate_target_dirname_exists(const char *target_dirname);

bool validate_arg_naming_convention(const char *arg_naming_convention);

#endif 
