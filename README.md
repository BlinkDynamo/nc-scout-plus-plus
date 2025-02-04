# nc-scout
A very minimal naming convention checker.

Finds files and directories that match (or don't match) a naming convention.

### Usage
The layout of a nc-scout function call.
nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]

Options:
    * -h, --help          Show this help message
    * -v, --version       Show the program version
    * -f, --full-path     Display matches by their full path name
    * -m, --matches       Print matches instead of non-matches

Commands:
    * search           Search for matches to a naming convention in a directory
        
Conventions:
    * camelcase        exampleFileName.txt
    * snakecase        example_file_name.txt
    * kebabcase        example-file-name.txt

### Dependencies
* make
* any C compiler (default is GCC) 
* POSIX-compliant system (Linux, MacOS)

### Installation
TODO
