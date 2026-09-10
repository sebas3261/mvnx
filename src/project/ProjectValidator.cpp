#include "ProjectValidator.h"
#include "ProjectNaming.h"

#include <regex>
#include <stdexcept>
#include <unordered_set>

using namespace std;

void ProjectValidator::validate(const ProjectConfig &config)
{
    validateProjectName(config.name, config.javaVersion);
    validateGroupId(config.groupId, config.javaVersion);
    validateJavaVersion(config.javaVersion);
}

void ProjectValidator::validateProjectName(const string &name, const string &javaVersion)
{
    regex pattern(R"(^[a-z][a-z0-9]*(-[a-z0-9]+)*$)");

    if (!regex_match(name, pattern))
    {
        throw invalid_argument("Invalid project name. Use lowercase letters, numbers and hyphens.");
    }

    string packageName = ProjectNaming::toPackageName(name);

    if (!isValidJavaIdentifier(packageName, javaVersion))
    {
        if (isJavaReservedWord(packageName))
        {
            throw invalid_argument("Invalid project name. '" + packageName + "' is a reserved Java keyword.");
        }

        throw invalid_argument("Invalid project name. Use lowercase letters, numbers and hyphens.");
    }
}

void ProjectValidator::validateGroupId(const string &groupId, const string &javaVersion)
{
    string segment;

    for (char c : groupId)
    {
        if (c == '.')
        {
            if (segment.empty())
            {
                throw invalid_argument("Invalid group ID. Package segments cannot be empty.");
            }

            if (isJavaReservedWord(segment))
            {
                throw invalid_argument("Invalid group ID. '" + segment + "' is a reserved Java keyword.");
            }

            if (!isValidJavaIdentifier(segment, javaVersion))
            {
                throw invalid_argument("Invalid group ID. Use lowercase package segments separated by dots.");
            }

            segment.clear();
        }
        else
        {
            segment += c;
        }
    }

    if (segment.empty())
    {
        throw invalid_argument("Invalid group ID. Package segments cannot be empty.");
    }

    if (isJavaReservedWord(segment))
    {
        throw invalid_argument("Invalid group ID. '" + segment + "' is a reserved Java keyword.");
    }

    if (!isValidJavaIdentifier(segment, javaVersion))
    {
        throw invalid_argument("Invalid group ID. Use lowercase package segments separated by dots.");
    }
}

void ProjectValidator::validateJavaVersion(const string &version)
{
    if (!isValidJavaVersion(version))
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
        "true", "false", "null",
        "exports", "module", "non-sealed", "open", "opens",
        "permits", "provides", "record", "requires", "sealed",
        "to", "transitive", "uses", "var", "with", "yield"};

    return reservedWords.contains(value);
}
