#include "HelpPrinter.h"
#include "cli/Style.h"

#include <iostream>

using namespace std;

void HelpPrinter::printGlobal()
{
    cout
        << "A modern developer experience for Maven.\n\n"

        << Style::CYAN
        << Style::BOLD
        << "Usage:"
        << Style::RESET
        << "\n"

        << "  mvnx <command> [options]\n\n"

        << Style::CYAN
        << Style::BOLD
        << "Commands:"
        << Style::RESET
        << "\n"

        << "  "
        << Style::GREEN
        << "init"
        << Style::RESET
        << "    Create a new Maven project\n"

        << "  "
        << Style::GREEN
        << "add"
        << Style::RESET
        << "     Add a dependency to the project\n\n"

        << Style::CYAN
        << Style::BOLD
        << "Options:"
        << Style::RESET
        << "\n"

        << "  "
        << Style::YELLOW
        << "-h, --help"
        << Style::RESET
        << "       Show help\n"

        << "  "
        << Style::YELLOW
        << "-v, --version"
        << Style::RESET
        << "    Show version\n";
}