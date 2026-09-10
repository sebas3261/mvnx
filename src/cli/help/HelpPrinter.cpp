#include "HelpPrinter.h"
#include "cli/Style.h"

#include <iostream>

using namespace std;

void HelpPrinter::printGlobal()
{
    cout
        << "\n"
        << Style::CYAN
        << "◆ "
        << Style::RESET
        << Style::BOLD
        << "mvnx"
        << Style::RESET
        << "\n\n"

        << "  A modern developer experience for Maven.\n\n"

        << Style::CYAN
        << "◇ "
        << Style::RESET
        << Style::BOLD
        << "Usage"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│  "
        << Style::RESET
        << "mvnx <command> [options]\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n\n"

        << Style::CYAN
        << "◇ "
        << Style::RESET
        << Style::BOLD
        << "Commands"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "init"
        << Style::RESET
        << "       Create a new Maven project\n"

        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "add"
        << Style::RESET
        << "        Add a dependency to the project\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n\n"

        << Style::CYAN
        << "◇ "
        << Style::RESET
        << Style::BOLD
        << "Options"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n"

        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "-h, --help"
        << Style::RESET
        << "       Show help\n"

        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "-v, --version"
        << Style::RESET
        << "    Show version\n"

        << Style::DIM
        << "│"
        << Style::RESET
        << "\n\n";
}

void HelpPrinter::printInit()
{
    cout
        << "\n"
        << Style::CYAN
        << "◆ "
        << Style::RESET
        << Style::BOLD
        << "mvnx init"
        << Style::RESET
        << "\n\n"

        << "  Create a new Maven project.\n\n"

        << Style::CYAN
        << "◇ "
        << Style::RESET
        << Style::BOLD
        << "Usage"
        << Style::RESET
        << "\n"
        << Style::DIM
        << "│  "
        << Style::RESET
        << "mvnx init [project-name] [options]\n\n"

        << Style::CYAN
        << "◇ "
        << Style::RESET
        << Style::BOLD
        << "Options"
        << Style::RESET
        << "\n"
        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "-g, --group-id <id>"
        << Style::RESET
        << "    Set Maven groupId\n"
        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "-j, --java <version>"
        << Style::RESET
        << "    Set Java version\n"
        << Style::DIM
        << "│  "
        << Style::RESET
        << Style::CYAN
        << "-h, --help"
        << Style::RESET
        << "              Show init help\n\n";
}
