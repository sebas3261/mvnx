#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "Option.h"

class ArgumentParser {
private:
    int argc;
    char** argv;
    int startIndex;

    std::vector<std::string> arguments;
    std::vector<Option> definitions;
    std::unordered_map<std::string, std::string> options;

    const Option* findLongOption(const std::string& name) const;
    const Option* findShortOption(char shortName) const;

public:
    ArgumentParser(int argc, char* argv[], int startIndex);

    void addOption(const Option& option);
    void parse();

    std::string getArgument(int index) const;
    std::string getOption(const std::string& name) const;
    bool hasOption(const std::string& name) const;
};