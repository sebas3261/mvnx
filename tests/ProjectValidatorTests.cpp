#include "project/ProjectValidator.h"

#include <initializer_list>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
int failures = 0;

void expectValidation(const ProjectConfig &config, bool expectedValid)
{
    bool valid = true;

    try
    {
        ProjectValidator::validate(config);
    }
    catch (const std::invalid_argument &)
    {
        valid = false;
    }
    catch (const std::exception &error)
    {
        std::cerr << "Unexpected exception for name='" << config.name
                  << "', groupId='" << config.groupId
                  << "', java='" << config.javaVersion
                  << "': " << error.what() << '\n';
        ++failures;
        return;
    }

    if (valid != expectedValid)
    {
        std::cerr << "Expected " << (expectedValid ? "acceptance" : "rejection")
                  << " for name='" << config.name
                  << "', groupId='" << config.groupId
                  << "', java='" << config.javaVersion << "'\n";
        ++failures;
    }
}
}

int main()
{
    for (const std::string version : {"8", "11", "17", "21", "25"})
    {
        for (const std::string groupId : {"_", "_.example", "com._.example", "com._"})
        {
            expectValidation({"demo-app", groupId, version}, version == "8");
        }

        for (const std::string groupId : {
                 "com.example", "com._foo", "com.foo_bar", "com.__"})
        {
            expectValidation({"demo-app", groupId, version}, true);
        }

        for (const std::string groupId : {
                 "class.example", "com.class", "com.null", "com.true",
                 "", ".com", "com.", "com..example"})
        {
            expectValidation({"demo-app", groupId, version}, false);
        }

        expectValidation({"cl-ass", "com.example", version}, false);
    }

    for (const std::string version : {"", "9", "26"})
    {
        expectValidation({"demo-app", "com.example", version}, false);
    }

    if (failures != 0)
    {
        std::cerr << failures << " validation expectation(s) failed\n";
        return 1;
    }

    return 0;
}
