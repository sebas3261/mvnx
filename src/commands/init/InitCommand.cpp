#include "InitCommand.h"
#include "../../cli/arguments/ArgumentParser.h"

#include <cctype>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>

using namespace std;

namespace fs = std::filesystem;

static string toPackageName(const string &name)
{
    string packageName;

    for (int i = 0; i < static_cast<int>(name.length()); i++)
    {
        char c = name[i];

        if (c == '-')
        {
            if (i + 1 >= static_cast<int>(name.length()))
            {
                break;
            }

            i++;
            c = name[i];
            c = static_cast<char>(toupper(c));
        }

        packageName += c;
    }

    return packageName;
}

static bool isValidGroupId(const string &groupId)
{
    regex pattern(
        R"(^[a-zA-Z_][a-zA-Z0-9_]*(\.[a-zA-Z_][a-zA-Z0-9_]*)*$)");

    return regex_match(groupId, pattern);
}

static bool isValidProjectName(const string &name)
{
    regex pattern(
        R"(^[a-zA-Z0-9]+(-[a-zA-Z0-9]+)*$)");

    return regex_match(name, pattern);
}

static fs::path packageToPath(const string &packageName)
{
    fs::path path;
    string current;

    for (char c : packageName)
    {
        if (c == '.')
        {
            path /= current;
            current.clear();
        }
        else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        path /= current;
    }

    return path;
}

int InitCommand::execute(int argc, char *argv[])
{
    ArgumentParser parser(argc, argv, 2);

    parser.addOption({"group-id", 'g', true});
    parser.addOption({"java", 'j', true});
    parser.addOption({"interactive", 'i', false});

    parser.parse();

    string projectName = parser.getArgument(0);
    string groupId = parser.getOption("group-id");
    string javaVersion = parser.getOption("java");

    if (projectName.empty())
    {
        cout << "Project name: ";
        cin >> projectName;
    }

    if (groupId.empty())
    {
        cout << "Group ID: ";
        cin >> groupId;
    }

    if (javaVersion.empty())
    {
        javaVersion = "21";
    }

    if (!isValidProjectName(projectName))
    {
        cerr << "Invalid project name. Use letters, numbers and hyphens only.\n";
        cerr << "Example: my-project\n";
        return 1;
    }

    if (!isValidGroupId(groupId))
    {
        cerr << "Invalid group ID.\n";
        cerr << "Example: com.myproject\n";
        return 1;
    }

    string packageName = groupId + "." + toPackageName(projectName);

    cout << "Project: " << projectName << '\n';
    cout << "Group ID: " << groupId << '\n';
    cout << "Java: " << javaVersion << '\n';
    cout << "Package name: " << packageName << '\n';

    fs::path projectPath = projectName;
    fs::path packagePath = packageToPath(packageName);

    fs::create_directories(
        projectPath / "src" / "main" / "java" / packagePath);

    fs::create_directories(
        projectPath / "src" / "test" / "java" / packagePath);

    fs::path mainJavaPath =
        projectPath / "src" / "main" / "java" / packagePath / "Main.java";

    ofstream mainFile(mainJavaPath);

    mainFile
        << "package " << packageName << ";\n\n"
        << "public class Main {\n"
        << "    public static void main(String[] args) {\n"
        << "        System.out.println(\"Hello from " << projectName << "!\");\n"
        << "    }\n"
        << "}\n";

    mainFile.close();

    fs::path pomPath = projectPath / "pom.xml";

    ofstream pomFile(pomPath);

    pomFile
        << "<project xmlns=\"http://maven.apache.org/POM/4.0.0\"\n"
        << "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        << "         xsi:schemaLocation=\"http://maven.apache.org/POM/4.0.0 "
           "https://maven.apache.org/xsd/maven-4.0.0.xsd\">\n"
        << "    <modelVersion>4.0.0</modelVersion>\n\n"
        << "    <groupId>" << groupId << "</groupId>\n"
        << "    <artifactId>" << projectName << "</artifactId>\n"
        << "    <version>1.0-SNAPSHOT</version>\n\n"
        << "    <properties>\n"
        << "        <maven.compiler.release>" << javaVersion << "</maven.compiler.release>\n"
        << "        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>\n"
        << "    </properties>\n"
        << "</project>\n";

    pomFile.close();

    return 0;
}