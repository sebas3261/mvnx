#include <iostream>
#include "commands/init/InitCommand.h"
#include "commands/add/AddCommand.h"
#include "cli/help/HelpPrinter.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: mvnx <command>\n";
        return 0;
    }

    string command = argv[1];

    if (command == "--version" || command == "-v")
    {
        cout << "mvnx " << MVNX_VERSION << '\n';
        return 0;
    }
    else if (command == "--help" || command == "-h")
    {
        HelpPrinter::printGlobal();
        return 0;
    }
    else if (command == "init")
    {
        InitCommand initCommand;
        return initCommand.execute(argc, argv);
    }
    else if (command == "add")
    {
        AddCommand addCommand;
        return addCommand.execute(argc, argv);
    }

    cout << "Command not found\n";
    return 0;
}