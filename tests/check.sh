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

# Counters incremented by test functions.
tests_passed=0
tests_executed=0

function test_matching_for_convention() {
    local convention="$1"
    local n_expected_correct="$2"

    (( tests_executed++ ))

    # Search inside the test directory of a naming convention, ignoring the name of the test 
    # directory itself.
    n_observed_correct=$( \
        ${BUILD_DIR}/nc-scout search -m ${convention} ${TESTS_DIR}/${convention} | wc -l )
    
    if [ "$n_observed_correct" -eq "$n_expected_correct" ]; then
        printf "[${GREEN}✓${RESET}]"
        (( tests_passed++ ))
    else
        printf "[${RED}X${RESET}]"  
    fi
    # Make the output look pretty and aligned.
    printf " %-16s | %s | %s\n" \
           "$convention" \
           "matches expected: $n_expected_correct" \
           "matches observed: $n_observed_correct" 
}

#----------------------------------------------------------------------------------------------#
# Test Execution
#----------------------------------------------------------------------------------------------#
printf "\nTesting matching for naming conventions...\n\n"

test_matching_for_convention flatcase       10
test_matching_for_convention camelcase      10
test_matching_for_convention pascalcase     10
test_matching_for_convention snakecase      10
test_matching_for_convention constantcase   10
test_matching_for_convention kebabcase      10
test_matching_for_convention cobolcase      10

printf "\nTesting completed. %s/%s tests were successful.\n\n" "$tests_passed" "$tests_executed"
