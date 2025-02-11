#!/usr/bin/env bash

# This file should only be executed from the project root (preferably by the Makefile).
if [ $(basename "$PWD") != "nc-scout" ]; then
    printf "Error: build-tests.sh should not be executed manually. If manual execution is required, do it from the project root.\n"
    exit 1
fi

tests_dir_structure_file="data/tests-dir-structure"

# Extract directory path lines while ignoring comment lines until the end of file.
directories=$(grep -v '^#' "$tests_dir_structure_file" | grep -v ' = ')

# Create test directory structure silently.
for dir in $directories; do
    mkdir -p "tests/$dir" 2>/dev/null
done

# Define colors and test() function.

GREEN="\033[0;32m"
RED="\033[0;31m"
RESET="\033[0m"

function test_matching_for_convention() {
    local naming_convention="$1"
    local n_expected_correct="$2"

    n_observed_correct=$(build/nc-scout search -m $naming_convention tests/ | wc -l)
    if [ "$n_observed_correct" == "$n_expected_correct" ]; then
        printf "[${GREEN}✓${RESET}]"
    else
        printf "[${RED}X${RESET}]"  
    fi
    printf " $naming_convention    $n_expected_correct matches expected    $n_observed_correct matches observed\n"
}

# Begin tests.
printf "\nTest matching for naming conventions...\n\n"
test_matching_for_convention camelcase 10
test_matching_for_convention kebabcase 10
test_matching_for_convention snakecase 10

