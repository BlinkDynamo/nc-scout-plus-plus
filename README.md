# ![nc-scout](img/nc-scout.png)

## Index
* [Description](#description)
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build](#build-instructions)
* [Installation](#installation)

## Description
nc-scout is a simple naming convention checker tool. It allows you to search directories for non-matching filenames to a naming convention. It is a personal tool I wanted for system cleanliness that became a larger project. It currently only supports predefined regular expressions as defined [here](src/search.c), although, I have played with the idea of creating a configuration file based approach, where you could create a much more customized search regimen.

## Dependencies
* make
* POSIX-compliant system (Linux, MacOS)

## Usage
The layout of a nc-scout command.

nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]

### Options
| Flag              | Description                                                |
|-------------------|------------------------------------------------------------|
| `-h, --help`      | Show a helpful message.                                    |
| `-v, --version`   | Show what version of the program you are using.            |
| `-f, --full-path` | Display matches by their full path name.                   |
| `-m, --matches`   | Print matches instead of non-matches.                      |

### Commands
|Command            | Description                                                |
|-------------------|------------------------------------------------------------| 
| `search`          | Search for matches to a naming convention in a directory.  |


### Conventions
| Convention        | Example                                                    |
|-------------------|------------------------------------------------------------|
| `flatcase`        | examplefilename.txt                                        |
| `camelcase`       | exampleFileName.txt                                        |
| `pascalcase`      | ExampleFileName.txt                                        |
| `snakecase`       | example_file_name.txt                                      |
| `constantcase`    | EXAMPLE_FILE_NAME.txt                                      |
| `kebabcase`       | example-file-name.txt                                      |
| `cobolcase`       | EXAMPLE-FILE-NAME.txt                                      |

## Build Instructions
**To begin, go to the root of the repository:**
```bash
cd nc-scout/
```

**Build the binary:**
```bash
make
``` 

**Run tests (optional):**
```bash
make check
```

## Installation
If the project built successfully, you can now either use the binary directly from the repository, or install it to your system.

**To use the binary directly:**
```bash
cd build/
./nc-scout --help
```

**To install the binary and use it systemwide:**
```bash
sudo make install
nc-scout --help
```

**Should you want to clean the build/ and tests/ directories:**
```bash
make clean
```

**To uninstall the binary:**
```bash
sudo make uninstall
```
