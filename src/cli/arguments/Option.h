#pragma once

#include <string>

struct Option {
    std::string name;
    char shortName;
    bool requiresValue;
};