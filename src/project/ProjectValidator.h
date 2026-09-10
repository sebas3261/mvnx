#pragma once

#include "ProjectConfig.h"

class ProjectValidator
{
public:
    static void validate(const ProjectConfig &config);
    static void validateProjectName(const std::string &name, const std::string &javaVersion);
    static void validateGroupId(const std::string &groupId, const std::string &javaVersion);
    static void validateJavaVersion(const std::string &version);
private:
    static bool isValidProjectName(const std::string &name, const std::string &javaVersion);
    static bool isValidGroupId(const std::string &groupId, const std::string &javaVersion);
    static bool isValidJavaVersion(const std::string &version);
    static bool isValidJavaIdentifier(const std::string &value, const std::string &javaVersion);
    static bool isJavaReservedWord(const std::string &value);
};
