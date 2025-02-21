/**********************************************************************************************
*
*   version - The current version of nc-scout.  
*
*   LICENSE: zlib/libpng 
*
*   Copyright (c) 2024-2025 Josh Hayden (@BlinkDynamo)
*
*   This software is provided ‘as-is’, without any express or implied
*   warranty. In no event will the authors be held liable for any damages
*   arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose,
*   including commercial applications, and to alter it and redistribute it
*   freely, subject to the following restrictions:
*
*   1. The origin of this software must not be misrepresented; you must not
*   claim that you wrote the original software. If you use this software
*   in a product, an acknowledgment in the product documentation would be
*   appreciated but is not required.
*
*   2. Altered source versions must be plainly marked as such, and must not be
*   misrepresented as being the original software.
*
*   3. This notice may not be removed or altered from any source
*   distribution. 
*
*********************************************************************************************/

#ifndef HELP_H
#define HELP_H

#define HELP_MESSAGE \
"Usage: nc-scout [OPTION]? [COMMAND] [CONVENTION] [DIRECTORY]\n" \
"\n" \
"Options:\n" \
"  -h, --help       Show this help message\n" \
"  -v, --version    Show the program version\n" \
"  -f, --full-path  Display files as a full path\n" \
"  -m, --matches    Display files that match the provided naming convention\n" \
"  -s, --strict     Enforces the naming convention strictly\n" \
"\n" \
"Commands:\n" \
"  search           Search for filenames that do not match a naming convention in a directory\n" \
"\n" \
"Conventions:\n" \
"  flatcase         examplefilename.txt\n" \
"  camelcase        exampleFileName.txt\n" \
"  pascalcase       ExampleFileName.txt\n" \
"  snakecase        example_file_name.txt\n" \
"  constantcase     EXAMPLE_FILE_NAME.txt\n" \
"  kebabcase        example-file-name.txt\n" \
"  cobolcase        EXAMPLE-FILE-NAME.txt\n" \
"\n" \
"Strict vs. Lenient:\n" \
"  The default enforcement of naming conventions for a search is lenient, although, using\n" \
"  the -s or --strict option, you can strictly enforce the naming convention for that search.\n" \
"\n" \
"  Strict enforcement means that the naming convention MUST be present in it's entirety, while\n" \
"  lenient enforcement means that the naming convention COULD be present in it's entirety if more\n" \
"  text is added, but not removed or changed.\n" \
"\n" \
"  Example:\n" \
"    example.txt\n" \
"\n" \
"    Matches strictly:\n" \
"      flatcase - Exactly matches the convention in it's entirety\n" \
"\n" \
"    Matches leniently:\n" \
"      kebabcase - Could be extended to example-file.txt to match the convention in it's entirety\n" \
"      snakecase - Could be extended to example_file.txt to match the convention in it's entirety\n" \
"      camelcase - Could be extended to exampleFile.txt to match the convention in it's entirety\n" \
"\n" \
""

#endif
