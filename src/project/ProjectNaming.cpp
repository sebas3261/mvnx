#include "ProjectNaming.h"

using namespace std;

string ProjectNaming::toPackageName(const string &name){
    string packageName;

    for (int i = 0; i < static_cast<int>(name.length()); i++)
    {
        char c = name[i];

        if (c == '-')
        {
            continue;
        }

        packageName += c;
    }

    return packageName;
}