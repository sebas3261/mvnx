#include "ProjectGenerator.h"
#include "ProjectNaming.h"

#include <filesystem>
#include <fstream>
#include <stdexcept>

namespace fs = std::filesystem;

static fs::path packageToPath(const std::string &packageName)
{
    fs::path path;
    std::string current;

    for (char c : packageName)
    {
        if (c == '.')
        {
            path /= current;
            current.clear();
        }
        else
        {
            current += c;
        }
    }

    if (!current.empty())
    {
        path /= current;
    }

    return path;
}

void ProjectGenerator::generate(const ProjectConfig &config)
{
    std::string packageName =
        config.groupId + "." +
        ProjectNaming::toPackageName(config.name);

    fs::path projectPath = config.name;

    if (fs::exists(projectPath))
    {
        throw std::invalid_argument(
            "Project directory already exists: " + config.name
        );
    }

    fs::path packagePath = packageToPath(packageName);

    fs::create_directories(
        projectPath /
        "src" /
        "main" /
        "java" /
        packagePath
    );

    fs::create_directories(
        projectPath /
        "src" /
        "test" /
        "java" /
        packagePath
    );

    fs::path mainJavaPath =
        projectPath /
        "src" /
        "main" /
        "java" /
        packagePath /
        "Main.java";

    std::ofstream mainFile(mainJavaPath);

    if (!mainFile.is_open())
    {
        throw std::runtime_error(
            "Could not create Main.java"
        );
    }

    mainFile
        << "package " << packageName << ";\n\n"
        << "public class Main {\n"
        << "    public static void main(String[] args) {\n"
        << "        System.out.println(\"Hello from "
        << config.name
        << "!\");\n"
        << "    }\n"
        << "}\n";

    mainFile.close();

    fs::path pomPath = projectPath / "pom.xml";

    std::ofstream pomFile(pomPath);

    if (!pomFile.is_open())
    {
        throw std::runtime_error(
            "Could not create pom.xml"
        );
    }

    pomFile
        << "<project xmlns=\"http://maven.apache.org/POM/4.0.0\"\n"
        << "         xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        << "         xsi:schemaLocation=\"http://maven.apache.org/POM/4.0.0 "
        << "https://maven.apache.org/xsd/maven-4.0.0.xsd\">\n"

        << "    <modelVersion>4.0.0</modelVersion>\n\n"

        << "    <groupId>"
        << config.groupId
        << "</groupId>\n"

        << "    <artifactId>"
        << config.name
        << "</artifactId>\n"

        << "    <version>1.0-SNAPSHOT</version>\n\n"

        << "    <properties>\n"

        << "        <maven.compiler.release>"
        << config.javaVersion
        << "</maven.compiler.release>\n"

        << "        <project.build.sourceEncoding>"
        << "UTF-8"
        << "</project.build.sourceEncoding>\n"

        << "    </properties>\n"

        << "</project>\n";

    pomFile.close();
}