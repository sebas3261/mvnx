#include "ProjectValidator.h"
#include "ProjectNaming.h"

#include <regex>
#include <stdexcept>
#include <unordered_set>

using namespace std;

void ProjectValidator::validate(const ProjectConfig &config)
{
    if (!isValidProjectName(config.name, config.javaVersion))
        throw invalid_argument("Invalid project name. Use lowercase letters, numbers and hyphens.");

    if (!isValidGroupId(config.groupId, config.javaVersion))
        throw invalid_argument("Invalid group ID. Example: com.example");

    if (!isValidJavaVersion(config.javaVersion))
        throw invalid_argument("Invalid Java version. Supported: 8, 11, 17, 21, 25");
}

bool ProjectValidator::isValidProjectName(const string &name, const string &javaVersion)
{
    ProjectNaming projectNaming;
    regex pattern(R"(^[a-z][a-z0-9]*(-[a-z0-9]+)*$)");

    return regex_match(name, pattern) && isValidJavaIdentifier(projectNaming.toPackageName(name), javaVersion);
}

bool ProjectValidator::isValidGroupId(const std::string &groupId, const std::string &javaVersion)
{
    string segment;

    for (char c : groupId)
    {
        if (c == '.')
        {
            if (!isValidJavaIdentifier(segment, javaVersion))
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

    return isValidJavaIdentifier(segment, javaVersion);
}

bool ProjectValidator::isValidJavaVersion(const string &version)
{
    static const unordered_set<string> supportedVersions = {
        "8", "11", "17", "21", "25"};

    return supportedVersions.contains(version);
}

bool ProjectValidator::isValidJavaIdentifier(const std::string &value, const std::string &javaVersion)
{
    // A single underscore became a keyword in Java 9; Java 8 still allows it.
    if (value == "_" && javaVersion != "8")
        return false;

    regex pattern(R"(^[a-z_][a-z0-9_]*$)");

    return regex_match(value, pattern) && !isJavaReservedWord(value);
}

bool ProjectValidator::isJavaReservedWord(const std::string &value)
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
