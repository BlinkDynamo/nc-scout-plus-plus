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
ORANGE="\033[0;33m"
RESET="\033[0m"

# Counters incremented by test functions.
tests_passed=0
tests_executed=0

function check_search()
{
    local test_name="$1"            # The name of the test itself, as well as the directory the test is testing.
    local convention="$2"           # The naming convention being tested.
    local options="$3"              # Any options being passed to nc-scout-pp search.
    local n_expected_correct="$4"   # The expected number of lines of output from the command.

    (( tests_executed++ ))

    # Search inside the test directory of a naming convention, ignoring the name of the test directory itself.
    local n_observed_correct=$( \
        ${BUILD_DIR}/nc-scout++ search ${options} ${convention} ${TESTS_DIR}/${test_name} | wc -l )
    
    if [ "$n_observed_correct" -eq "$n_expected_correct" ]; then
        printf "[${GREEN}✓${RESET}]"
        (( tests_passed++ ))
    else
        printf "[${RED}X${RESET}]"  
    fi
    # Make the output look pretty and aligned.
    printf " %-40s | %s | %s\n" \
           "${TESTS_DIR}/${test_name}" \
           "matches expected: $n_expected_correct" \
           "matches observed: $n_observed_correct" 
}

#----------------------------------------------------------------------------------------------#
# Test Execution
#----------------------------------------------------------------------------------------------#
printf "\nTesting 'search' subcommand.\n\n"

printf "${ORANGE}Tests for strict matches:${RESET}\n"
check_search "flatcase_strict_matches" "flatcase" "-sm" 10
check_search "camelcase_strict_matches" "camelcase" "-sm" 10
check_search "pascalcase_strict_matches" "pascalcase" "-sm" 10
check_search "snakecase_strict_matches" "snakecase" "-sm" 10
check_search "constantcase_strict_matches" "constantcase" "-sm" 10
check_search "kebabcase_strict_matches" "kebabcase" "-sm" 10
check_search "cobolcase_strict_matches" "cobolcase" "-sm" 10
printf "\n"

printf "${ORANGE}Tests for lenient matches:${RESET}\n"
check_search "flatcase_lenient_matches" "flatcase" "-m" 10
check_search "camelcase_lenient_matches" "camelcase" "-m" 10
check_search "pascalcase_lenient_matches" "pascalcase" "-m" 10
check_search "snakecase_lenient_matches" "snakecase" "-m" 10
check_search "constantcase_lenient_matches" "constantcase" "-m" 10
check_search "kebabcase_lenient_matches" "kebabcase" "-m" 10
check_search "cobolcase_lenient_matches" "cobolcase" "-m" 10
printf "\n"

printf "\nTesting completed. %s/%s tests were successful.\n\n" "$tests_passed" "$tests_executed"
