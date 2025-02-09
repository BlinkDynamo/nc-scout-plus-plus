![nc-scout](/img/nc-scout.png)
### A very minimal naming convention checker.


## Dependencies
make
POSIX-compliant system (Linux, MacOS)

## Usage

The layout of a nc-scout function call.

nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]
#
### Options

|||
|--|--|
| -h, --help |  Show a help message with more information about the program. |
| -v, --version | Show what version of the program you are using. |
| -f, --full-path | Display matches by their full path name |
| -m, --matches | Print matches instead of non-matches |
|||
#
### Commands
|||
|--|--| 
| search | Search for matches to a naming convention in a directory |
|||
#
### Conventions
Conventions:
camelcase exampleFileName.txt
snakecase example_file_name.txt
kebabcase example-file-name.txt


## Build Instructions

To begin, go to the project root: `nc-scout/`

  

Choose one of these build options:

1. Simple build:
`make`

  

2. Run tests and build:
`make check`

## Installation
If the project built successfully, you can now either use the binary directly from the repository, or install it to your system.

1. Using the binary directly:
`cd build/`
`./nc-scout --help`

2. Installing the binary (currently unimplemented):
`make install`

## Cleaning
Should you want to clean the build/ and tests/ directories, run:
`make clean`


