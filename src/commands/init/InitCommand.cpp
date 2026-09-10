#include "InitCommand.h"
#include "cli/arguments/ArgumentParser.h"

#include <filesystem>
#include <iostream>
#include <regex>
#include <string>
#include <fstream>
#include <unordered_set>

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
            continue;
        }

        packageName += c;
    }

    return packageName;
}

static bool isJavaReservedWord(const string &value)
{
    static const unordered_set<string> reservedWords = {
        "abstract", "assert", "boolean", "break", "byte",
        "case", "catch", "char", "class", "const",
        "continue", "default", "do", "double", "else",
        "enum", "extends", "final", "finally", "float",
        "for", "goto", "if", "implements", "import",
        "instanceof", "int", "interface", "long", "native",
        "new", "package", "private", "protected", "public",
        "return", "short", "static", "strictfp", "super",
        "switch", "synchronized", "this", "throw", "throws",
        "transient", "try", "void", "volatile", "while",
        "true", "false", "null"};

    return reservedWords.contains(value);
}

static bool isValidJavaIdentifier(const string &value)
{
    regex pattern(R"(^[a-z_][a-z0-9_]*$)");

    return regex_match(value, pattern) && !isJavaReservedWord(value);
}

static bool isValidGroupId(const string &groupId)
{
    string segment;

    for (char c : groupId)
    {
        if (c == '.')
        {
            if (!isValidJavaIdentifier(segment))
            {
                return false;
            }

            segment.clear();
        }
        else
        {
            segment += c;
        }
    }

    return isValidJavaIdentifier(segment);
}

static bool isValidProjectName(const string &name)
{
    regex pattern(R"(^[a-z][a-z0-9]*(-[a-z0-9]+)*$)");

    return regex_match(name, pattern) && isValidJavaIdentifier(toPackageName(name));
}

static bool isValidJavaVersion(const string &version)
{
    static const unordered_set<string> supportedVersions = {
        "8", "11", "17", "21", "25"};

    return supportedVersions.contains(version);
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

    if (!isValidProjectName(projectName) || isJavaReservedWord(projectName))
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

    if (!isValidJavaVersion(javaVersion))
    {
        cerr << "Invalid Java version. Supported versions: 8, 11, 17, 21, 25.\n";
        cerr << "Example: --java 21\n";
        return 1;
    }

    string packageName = groupId + "." + toPackageName(projectName);

    cout << "Project: " << projectName << '\n';
    cout << "Group ID: " << groupId << '\n';
    cout << "Java: " << javaVersion << '\n';
    cout << "Package name: " << packageName << '\n';

    fs::path projectPath = projectName;

    if (fs::exists(projectPath))
    {
        cerr << "Project directory already exists: " << projectName << '\n';
        return 1;
    }

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

    if (!pomFile.is_open())
    {
        cerr << "Error: could not create pom.xml\n";
        return 1;
    }

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
