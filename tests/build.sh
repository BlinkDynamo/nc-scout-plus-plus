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
tests_dir_structure_file="tests/data/directories"

# Extract directory path lines ignoring empty lines.
directories=$(awk 'NF' "$tests_dir_structure_file")

#----------------------------------------------------------------------------------------------#
# Build
#----------------------------------------------------------------------------------------------#

# Create test directory structure silently.
for dir in $directories; do
    mkdir -p "${TESTS_DIR}/${dir}" 2>/dev/null
done
