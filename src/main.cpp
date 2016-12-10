// Title: SCEW (Simple Console Emulator for Windows)
// Author: dandreas
// Description: 'Emulates' a windows console, useful for systems that restrict the use of cmd.exe.

#include <iostream>
#include <iomanip>
#include <windows.h> // for system()
#include <direct.h> // finds current working directory
#include <string> // for string functions
#include <sstream>
#include <vector> // for vectors

#include "config.h" // Everything for .ascew files

using namespace std;

int main(int argc, char* argv[])
{
    // Startup command checks
    bool bBasic = false; // if true, runs the most basic console emulator, skips reading any configuration files, etc.
    bool bNoText = false; // if true, hides the opening text
    
    if(argc > 1)
    {
        for(int i = 0;i < argc;i++)
        {
            string sArg = std::string(argv[i]);
            
            if(DEBUG)
            {
                cout << "Args: " << sArg << endl;
            }
            
            if(sArg == "-h")
            {
                // Eventually, a help function, currently just displays help
                system("help");
                return 0;
            }
            else if(sArg == "-b")
            {
                bBasic = true;
            }
            else if(sArg == "-nt")
            {
                bNoText = true;
            }
                
        }
    }

    // End startup
    HANDLE hConsole; // Handle for console colors
    if(!bBasic)
    {
        hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    }
    
    const double VERSION = 1.30; // Build version number. Patch updates aren't displayed
    const double UPDATE = 3; // Updates since last version
    
    const std::string AUTHOR = "dandreas (GitHub)";

    _path pPath;
    
    if(!bBasic)
    {
        pPath = SetPath(); // Grabs settings from the local settings file
    }
    
    std::string sInput = ""; // Stores the user input
    std::string sTitle = ""; // Title for the application
    std::string sDirectory = ""; // Stores the working directory
    
    sDirectory = _getcwd(NULL, 0); // Gets the directory
    sTitle = "ascew " + sDirectory;
    SetConsoleTitle(sTitle.c_str()); // Sets the console name

    if(!bBasic)
    {
        SetConsoleTextAttribute(hConsole,pPath.iColor);
        system("cls");
    }

    if(!bNoText)
    {
        cout << "A Simple Console Emulator for Windows (ASCEW) v" << VERSION << "u" << UPDATE << endl
             << "Made by: " << AUTHOR
             << "\n" << "\n";
    }

    // Any pre-execution debug stuff should be put in here
    if (DEBUG)
    {
        if(!bBasic)
        {
            cout << "Number of custom commands: " <<  pPath.count << "\n";
        }
        else
        {
            cout << "Custom commands are disabled." << endl;
        }
    }
    
    while(true)
    {
        bool bCustom = false; // Tells if a custom command is being used
        
        if(!bBasic)
        {
            SetConsoleTextAttribute(hConsole,pPath.iDircolor);
        }
        cout << sDirectory << ">";
        if(!bBasic)
        {
            SetConsoleTextAttribute(hConsole,pPath.iColor);
        }
        getline(cin,sInput);

        if(!bBasic)
        {
            // If there isn't a config file, skip the whole jazz
            if(pPath.bIsActive)
            {
                // Only checks the actual command
                stringstream ss;
                ss << sInput;
                string sTemp;
                ss >> sTemp;
                // Tests if the command is a custom made command
                for(int i = 0; i < pPath.count; i++)
                {
                    if(sTemp == pPath.sAlias[i])
                    {
                        bCustom = true; // Switches the custom option on
                        sInput = pPath.sExecutable[i]; // Auto-sets the user input to the location of the executable
                        while(ss)
                        {
                            ss >> sTemp;
                            sInput += " " + sTemp;
                        }
                    }
                }
            }
        }
        
        // Runs the appropriate function per the user's input
        if(bCustom)
        {
            // Runs the user's specified executable
            system(sInput.c_str());
        }
        else if(sInput[0] == 'c' && sInput[1] == 'd' && sInput[2] == ' ')
        {
            // cd clone, created because system("cd <dir>") won't change the programs working directory.
            try
            {
                sInput.erase(0,3); // Removes "cd " from the string
                SetCurrentDirectory(sInput.c_str()); // Sets the working directory

                // Set the directory variable, reset console title
                sDirectory = _getcwd(NULL, 0);
                sTitle = "ascew " + sDirectory;
                SetConsoleTitle(sTitle.c_str());

                cout << sDirectory << endl; // emulate actual cd output
            }
            catch(...)
            {
                cout << "Directory Change failed. Input: " << sInput << endl;
            }
        }
        else if(sInput == "exit")
        {
            // exit clone, created because system("exit") won't end the actual program.
            return 0;
        }
        else
        {
            // Runs the user input in cmd (hopefully) bypassing any console locks
            system(sInput.c_str());
        }
        
        cout << endl;
    }
}
