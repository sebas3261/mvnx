#pragma once
#include "project/ProjectConfig.h"

class InitCommand{
    public:
        int execute(int argc, char* argv[]);
    
    private:
        ProjectConfig collectConfig(int argc, char *argv[]);
};