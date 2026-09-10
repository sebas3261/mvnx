#pragma once

#include <replxx.hxx>
#include <string>
#include <vector>

class Prompt
{
private:
    replxx::Replxx rx;

public:
    std::string text(
        const std::string &label,
        const std::string &defaultValue = "",
        bool preload = false
    );

    std::string select(
        const std::string &label,
        const std::vector<std::string> &options,
        const std::string &defaultValue = ""
    );
};
