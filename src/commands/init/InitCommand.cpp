#include "InitCommand.h"

#include "cli/arguments/ArgumentParser.h"
#include "cli/prompt/Prompt.h"

#include "project/ProjectConfig.h"
#include "project/ProjectGenerator.h"
#include "project/ProjectValidator.h"

#include <exception>
#include <iostream>
#include <string>

using namespace std;

ProjectConfig InitCommand::collectConfig(int argc, char *argv[])
{
    ArgumentParser parser(argc, argv, 2);

    parser.addOption({"group-id", 'g', true});
    parser.addOption({"java", 'j', true});
    parser.addOption({"interactive", 'i', false});

    parser.parse();

    string projectName = parser.getArgument(0);
    string groupId = parser.getOption("group-id");
    string javaVersion = parser.getOption("java");

    Prompt prompt;

    if (projectName.empty())
    {
        projectName = prompt.text(
            "Project name",
            "my-project"
        );
    }

    if (groupId.empty())
    {
        groupId = prompt.text(
            "Group ID",
            "com.example"
        );
    }

    if (javaVersion.empty())
    {
        javaVersion = prompt.select(
            "Java version",
            {"8", "11", "17", "21", "25"}
        );
    }

    return {
        projectName,
        groupId,
        javaVersion
    };
}

int InitCommand::execute(int argc, char *argv[])
{
    cout << "\n◆ mvnx init\n\n";

    ProjectConfig config = collectConfig(argc, argv);

    if (config.name.empty() ||
        config.groupId.empty() ||
        config.javaVersion.empty())
    {
        cerr << "\nOperation cancelled.\n";
        return 1;
    }

    try
    {
        ProjectValidator::validate(config);

        cout << "\n◆ Creating Maven project\n\n";

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
        cerr << "\n✗ " << e.what() << '\n';
        return 1;
    }

    return 0;
}
