#ifndef CONFIG_H
#define CONFIG_H

#include <vector>

const bool DEBUG = false; // Enables debug mode to inspect the IO properties

// This struct stores the applications special path, set up in the .ascew file
typedef struct
{
    std::string sOrigin; // Stores the start directory of the application
    bool bIsActive; // Tells if there is a config file present
    int count; // The (base 1) amount of items in the sExecutable array
    std::vector<std::string> sExecutable; // Paths to the files in relation to to executable. eg ../../nano/nano.exe
    std::vector<std::string> sAlias; // Alias to the related executable, eg "nano" for the above example
    int iColor; // Color of user input/output text
    int iBgcolor; // Background color
    int iDircolor; // Directory output color
} _path;

_path SetPath(); // Sets up the _path struct using the .ascew file (if present)

#endif
