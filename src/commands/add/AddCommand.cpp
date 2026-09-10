#include "AddCommand.h"
#include "cli/arguments/ArgumentParser.h"
#include <iostream>

int AddCommand::execute(int argc, char *argv[])
{
    ArgumentParser parser(argc, argv, 2);

    parser.parse();

    const std::vector<std::string> &dependencies = parser.getArguments();

    for (const std::string &dependency : dependencies)
    {
        std::cout << dependency << '\n';
    }

    return 0;
}
