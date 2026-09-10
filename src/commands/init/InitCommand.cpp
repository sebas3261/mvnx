#include "InitCommand.h"

#include "cli/arguments/ArgumentParser.h"
#include "cli/help/HelpPrinter.h"
#include "cli/Style.h"
#include "cli/prompt/Prompt.h"

#include "project/ProjectConfig.h"
#include "project/ProjectGenerator.h"
#include "project/ProjectValidator.h"

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

ProjectConfig InitCommand::collectConfig(const ArgumentParser &parser)
{
    string projectName = parser.getArgument(0);
    string groupId = parser.getOption("group-id");
    string javaVersion = parser.getOption("java");
    bool hasProjectNameArgument = !parser.getArguments().empty();

    Prompt prompt;

    if (!javaVersion.empty())
    {
        ProjectValidator::validateJavaVersion(javaVersion);
    }

    if (hasProjectNameArgument)
    {
        ProjectValidator::validateProjectName(projectName, javaVersion.empty() ? "21" : javaVersion);
    }

    if (!groupId.empty())
    {
        ProjectValidator::validateGroupId(groupId, javaVersion.empty() ? "21" : javaVersion);
    }

    if (!hasProjectNameArgument)
    {
        projectName = prompt.text(
            "Project name",
            "my-project");

        if (projectName.empty())
        {
            return {};
        }
    }

    if (groupId.empty())
    {
        groupId = prompt.text(
            "Group ID",
            "com.example");

        if (groupId.empty())
        {
            return {};
        }
    }

    if (javaVersion.empty())
    {
        javaVersion = prompt.select(
            "Java version",
            {"8", "11", "17", "21", "25"},
            "21");

        if (javaVersion.empty())
        {
            return {};
        }
    }

    return {
        projectName,
        groupId,
        javaVersion};
}

int InitCommand::execute(int argc, char *argv[])
{
    ArgumentParser parser(argc, argv, 2);
    parser.addOption({"group-id", 'g', true});
    parser.addOption({"java", 'j', true});
    parser.addOption({"help", 'h', false});

    try
    {
        parser.parse();
    }
    catch (const exception &e)
    {
        cerr
            << Style::CYAN
            << "✗ "
            << Style::RESET
            << e.what()
            << '\n';
        return 1;
    }

    if (parser.hasOption("help"))
    {
        HelpPrinter::printInit();
        return 0;
    }

    if (parser.getArguments().size() > 1)
    {
        cerr
            << Style::CYAN
            << "✗ "
            << Style::RESET
            << "Too many arguments. Usage: mvnx init [project-name] [options]\n";
        return 1;
    }

    cout
        << '\n'
        << Style::CYAN
        << "◆"
        << Style::RESET
        << " mvnx init"
        << "\n\n"
        << flush;

    ProjectConfig config;

    try
    {
        config = collectConfig(parser);
    }
    catch (const exception &e)
    {
        cerr 
            << Style::CYAN
            << "✗ " 
            << Style::RESET
            << e.what() 
            << '\n';
        return 1;
    }

    if (config.name.empty() ||
        config.groupId.empty() ||
        config.javaVersion.empty())
    {
        cerr 
            << Style::CYAN
            << "✗ "
            << Style::RESET
            << "Operation cancelled.\n";
        return 1;
    }

    try
    {
        ProjectValidator::validate(config);

        cout
            << '\n'
            << Style::CYAN
            << "◆ Creating Maven project"
            << Style::RESET
            << "\n\n";

        cout << "  Project   " << config.name << '\n';
        cout << "  Group     " << config.groupId << '\n';
        cout << "  Java      " << config.javaVersion << '\n';

        ProjectGenerator::generate(config);

        cout << "\n✓ Project created successfully\n\n";

        cout << "  cd " << config.name << '\n';
        cout << "  mvn package\n\n";
    }
    catch (const exception &e)
    {
        cerr << "✗ " << e.what() << '\n';
        return 1;
    }

    return 0;
}
