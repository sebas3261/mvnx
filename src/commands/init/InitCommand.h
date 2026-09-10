#pragma once
#include "project/ProjectConfig.h"

class ArgumentParser;

class InitCommand{
    public:
        int execute(int argc, char* argv[]);
    
    private:
        ProjectConfig collectConfig(const ArgumentParser &parser);
};
