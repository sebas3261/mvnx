#include "ArgumentParser.h"

ArgumentParser::ArgumentParser(int argc, char* argv[], int startIndex)
    : argc(argc), argv(argv), startIndex(startIndex) {
}

void ArgumentParser::addOption(const Option& option) {
    definitions.push_back(option);
}

const Option* ArgumentParser::findLongOption(const std::string& name) const {
    for (const Option& option : definitions) {
        if (option.name == name) {
            return &option;
        }
    }

    return nullptr;
}

const Option* ArgumentParser::findShortOption(char shortName) const {
    for (const Option& option : definitions) {
        if (option.shortName == shortName) {
            return &option;
        }
    }

    return nullptr;
}

void ArgumentParser::parse() {
    for (int i = startIndex; i < argc; i++) {
        std::string current = argv[i];

        if (current.starts_with("--")) {
            std::string name = current.substr(2);

            const Option* option = findLongOption(name);

            if (option == nullptr) {
                continue;
            }

            if (option->requiresValue) {
                if (i + 1 < argc) {
                    options[option->name] = argv[i + 1];
                    i++;
                }
            } else {
                options[option->name] = "";
            }

        } else if (current.starts_with("-") && current.length() == 2) {
            char shortName = current[1];

            const Option* option = findShortOption(shortName);

            if (option == nullptr) {
                continue;
            }

            if (option->requiresValue) {
                if (i + 1 < argc) {
                    options[option->name] = argv[i + 1];
                    i++;
                }
            } else {
                options[option->name] = "";
            }

        } else {
            arguments.push_back(current);
        }
    }
}

std::string ArgumentParser::getArgument(int index) const {
    if (index < 0 || index >= static_cast<int>(arguments.size())) {
        return "";
    }

    return arguments[index];
}

const std::vector<std::string>& ArgumentParser::getArguments() const
{
    return arguments;
}

std::string ArgumentParser::getOption(const std::string& name) const {
    auto it = options.find(name);

    if (it == options.end()) {
        return "";
    }

    return it->second;
}

bool ArgumentParser::hasOption(const std::string& name) const {
    return options.contains(name);
}