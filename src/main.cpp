#include <iostream>
#include "commands/init/InitCommand.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: mvnx <command>\n";
        return 0;
    }

    string command = argv[1];

    if (command == "init")
    {
        InitCommand initCommand;
        return initCommand.execute(argc, argv);
    }

    cout << "Command not found\n";
    return 0;
}