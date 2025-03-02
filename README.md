# ![nc-scout++](img/nc-scout++.png)

## Index
* [Description](#description)
* [Dependencies](#dependencies)
* [Usage](#usage)
* [Build](#build-instructions)
* [Installation](#installation)

## Description
nc-scout-pp is a C++ recoded fork of nc-scout 0.3.0. It is a simple naming convention checker tool. It allows you to search directories for non-matching filenames to a naming convention. It is a personal tool I wanted for system cleanliness that became a larger project. It currently only supports predefined regular expressions as defined [here](src/naming.cpp), although, I have played with the idea of creating a configuration file based approach, where you could create a much more customized search regimen.

## Dependencies
* make
* POSIX-compliant system (Linux, MacOS)

## Usage
The layout of a nc-scout-pp command.

nc-scout-pp [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]

### Options
| Flag              | Description                                                              |
|-------------------|--------------------------------------------------------------------------|
| `-v, --version`   | Show what version of the program you are using.                          |
| `-h, --help`      | Show a helpful message.                                                  |
| `-f, --full-path` | Display the full path of matching files instead of relative paths.       |
| `-m, --matches`   | Print matches instead of non-matches.                                    |
| `-s, --strict`    | Search for the naming convention strictly.                               |
| `-R, --recursive` | Recursively search through subdirectories.                               |

### Commands
|Command   | Description                                                                       |
|----------|-----------------------------------------------------------------------------------| 
| `search` | Search a directory for files that do not match a naming convention.               |


### Conventions
| Convention        | Example                                                                  |
|-------------------|--------------------------------------------------------------------------|
| `camelcase`       | exampleFileName.txt                                                      |
| `cobolcase`       | EXAMPLE-FILE-NAME.txt                                                    |
| `constantcase`    | EXAMPLE_FILE_NAME.txt                                                    |
| `flatcase`        | examplefilename.txt                                                      |
| `kebabcase`       | example-file-name.txt                                                    |
| `pascalcase`      | ExampleFileName.txt                                                      |
| `snakecase`       | example_file_name.txt                                                    |

### What is the Filename Body of a Filename?
The **filename body** is the text of a file's full filename, ignoring leading periods and file extentions. The final period itself and the text that follows it is what is defined as the file extention. `search` is only performed on the filename body of a filename.

```bash
# Search for matches:
nc-scout-pp search snakecase --matches ./
# Output:
.example_file           # The leading period is ignored, resulting in the filename body 'example_file', which is snakecase.
example_file.txt        # The file extention '.txt' is ignored, resulting in the filename body 'example_file', which is snakecase.
.example_file.RAR       # The file extention '.RAR' is ignored, resulting in the filename body 'example_file', which is snakecase.

# Search for non-matches: 
nc-scout-pp search flatcase ./
# Output:
..example_file          # The leading period is ignored, resulting in the filename body '.example', which is not snakecase.
example_file.exe.txt    # The file extention '.txt' is ignored, resulting in the filename body 'example_file.exe', which is not snakecase.

```

### Strict vs. Lenient:
The default enforcement of naming conventions for a search is lenient, although, using
the `-s` or `--strict` option, you can strictly enforce the naming convention for that search.

Strict enforcement means that the naming convention **must** be present in it's entirety in the filename body, while lenient enforcement means that the naming convention **could** be present in it's entirety in the filename body if more text is added, but not removed or changed.

Example File: **example.txt** <em>(filename body is 'example')</em>

Strict matches:
* `flatcase` - The filename body <em>'example'</em> is all lowercase, so it strictly matches the convention.

Lenient matches:
* `kebabcase` - The filename body <em>'example'</em> could be extended to <em>'example-file'</em>, so it leniently matches the convention.
* `snakecase` - the filename body <em>'example'</em> could be extended to <em>'example_file'</em>, so it leniently matches the convention.
* `camelcase` - The filename body <em>'example'</em> could be extended to <em>'exampleFile'</em>, so it leniently matches the convention.

## Build Instructions
To begin, clone the project and go to the root of the repository:
```bash
git clone https://github.com/BlinkDynamo/nc-scout-pp.git nc-scout-pp
cd nc-scout-pp/
```

Build the binary:
```bash
make
``` 

Run tests (optional):
```bash
make check
```

## Installation
If the project built successfully, you can now either use the binary directly from the repository, or install it to your system.

To use the binary directly:
```bash
cd build/
# called directly from the build directory.
./nc-scout-pp --help
```

To install the binary and use it systemwide:
```bash
sudo make install
# called from /usr/local/bin/
nc-scout-pp --help
```

Should you want to clean the build/ and tests/ directories:
```bash
make clean
```

To uninstall the binary:
```bash
sudo make uninstall
```
