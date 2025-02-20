#!/usr/bin/env bash

# Copyright (c) 2024-2025 Josh Hayden (@BlinkDynamo)

# This file should only be executed by the Makefile because it defines these variables.
if [ -z ${BUILD_DIR} ] && [ -z ${TESTS_DIR} ]; then
    printf "%s\n%s\n" \
           "Error: build-tests.sh is a child script of the Makefile." \
           "It should not be executed manually."
    exit 1
fi

#----------------------------------------------------------------------------------------------#
# Definitions 
#----------------------------------------------------------------------------------------------#
build_test_directory() {
    directory_name="$1"         # The name of the directory that will be created.
    test_filenames_file="$2"    # The file of filenames in /tests/data that should be read.

    mkdir -p "${TESTS_DIR}/${directory_name}"

    # Read the filenames from test_filenames_file into an array of strings.
    mapfile -t test_filenames_array < "${test_filenames_file}"
    
    for file in "${test_filenames_array[@]}"; do
       touch "${TESTS_DIR}/${directory_name}/${file}" 
    done

    printf "Built test directory '%s' from data file '%s'.\n" \
           "${directory_name}" \
           "${test_filenames_file}"
}

#----------------------------------------------------------------------------------------------#
# Build
#----------------------------------------------------------------------------------------------#
printf "\nBuilding test directories...\n\n"

# Create test directory structure silently.
build_test_directory "flatcase" "tests/data/flatcase-filenames"
build_test_directory "camelcase" "tests/data/camelcase-filenames"
build_test_directory "pascalcase" "tests/data/pascalcase-filenames"
build_test_directory "kebabcase" "tests/data/kebabcase-filenames"
build_test_directory "cobolcase" "tests/data/cobolcase-filenames"
build_test_directory "snakecase" "tests/data/snakecase-filenames"
build_test_directory "constantcase" "tests/data/constantcase-filenames"

printf "\nTest directories built successfully.\n\n"
