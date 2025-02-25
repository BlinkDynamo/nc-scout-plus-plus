#!/usr/bin/env bash

# Copyright (c) 2024-2025 Josh Hayden (@BlinkDynamo)

# This file should only be executed by the Makefile because it defines these variables.
if [ -z ${BUILD_DIR} ] && [ -z ${TESTS_DIR} ]; then
    printf "%s\n%s\n" \
           "Error: build-tests.sh is a child script of the Makefile." \
           "It should not be executed manually."
    exit 1
fi

# Source the data file including arrays of test filenames, arranged by test.
source "tests/data.sh.inc"

#----------------------------------------------------------------------------------------------#
# Definitions 
#----------------------------------------------------------------------------------------------#
build_test_directory() {
    # The name of test being executed,  as well as the array name of it's needed data in tests/filenames.inc.
    testname="$1"

    mkdir -p "${TESTS_DIR}/${testname}"
    printf "Built test directory '%s'.\n" "${testname}"

    # Construct the variable name dynamically

    # Use eval to access the array
    eval "values=(\"\${${testname}[@]}\")"
    if eval "[[ -z \"\${${testname}[*]}\" ]]" 2>/dev/null; then
        echo "Unable to evaluate $testname to an array."
        return
    fi

    # Iterate over the values
    for item in "${values[@]}"; do
        touch "${TESTS_DIR}/${testname}/$item"
    done
}

#----------------------------------------------------------------------------------------------#
# Build
#----------------------------------------------------------------------------------------------#
printf "\nBuilding test directories...\n\n"

# Strict matches.
build_test_directory "flatcase_strict_matches"
build_test_directory "camelcase_strict_matches"
build_test_directory "pascalcase_strict_matches"
build_test_directory "kebabcase_strict_matches"
build_test_directory "cobolcase_strict_matches"
build_test_directory "snakecase_strict_matches"
build_test_directory "constantcase_strict_matches"

# Lenient matches
build_test_directory "flatcase_lenient_matches"
build_test_directory "camelcase_lenient_matches"
build_test_directory "pascalcase_lenient_matches"
build_test_directory "kebabcase_lenient_matches"
build_test_directory "cobolcase_lenient_matches"
build_test_directory "snakecase_lenient_matches"
build_test_directory "constantcase_lenient_matches"

printf "\nTest directories built successfully.\n\n"
