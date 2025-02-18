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
GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

tests_dir_structure_file="tests/data/tests-dir-structure"

# Extract directory path lines.
directories=$(awk 'NF' "$tests_dir_structure_file")

# Create test directory structure silently.
for dir in $directories; do
    mkdir -p "${TESTS_DIR}/${dir}" 2>/dev/null
done

function test_matching_for_convention() {
    local naming_convention="$1"
    local n_expected_correct="$2"

    n_observed_correct=$(${BUILD_DIR}/nc-scout search -m ${naming_convention} ${TESTS_DIR} | wc -l)
    if [ "$n_observed_correct" == "$n_expected_correct" ]; then
        printf "[${GREEN}✓${RESET}]"
    else
        printf "[${RED}X${RESET}]"  
    fi
    # Make the output look pretty and aligned.
    printf " %-16s | %s | %s\n" \
           "$naming_convention" \
           "matches expected: $n_expected_correct" \
           "matches observed: $n_observed_correct" 
}

#----------------------------------------------------------------------------------------------#
# Test Execution
#----------------------------------------------------------------------------------------------#
printf "\nTest matching for naming conventions...\n\n"

test_matching_for_convention flatcase       10
test_matching_for_convention camelcase      10
test_matching_for_convention pascalcase     10
test_matching_for_convention snakecase      10
test_matching_for_convention constantcase   10
test_matching_for_convention kebabcase      10
test_matching_for_convention cobolcase      10
