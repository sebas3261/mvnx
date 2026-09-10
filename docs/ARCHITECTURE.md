# mvnx — Architecture

This document describes the intended internal architecture of `mvnx`.

The architecture will evolve as the project grows.

The primary objective is to keep CLI concerns, Maven integration, dependency resolution, filesystem operations, and project generation separated.

---

# High-level architecture

```text
                  ┌──────────────────┐
                  │       CLI        │
                  │                  │
                  │ ArgumentParser   │
                  │ Prompt / Style   │
                  └────────┬─────────┘
                           │
                           ▼
                  ┌──────────────────┐
                  │     Commands     │
                  │                  │
                  │ init             │
                  │ add              │
                  │ remove           │
                  │ install          │
                  │ run              │
                  └────────┬─────────┘
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
      Project         Dependencies       JDK
      generation      & resolution    management
            │              │
            │              ▼
            │       Maven integration
            │              │
            └──────────────┼──────────────┐
                           ▼              ▼
                       Filesystem       Network
```

Commands coordinate operations.

They should not contain low-level implementations for every operation.

---

# Current structure

```text
src/
├── main.cpp
│
├── cli/
│   ├── arguments/
│   │   ├── ArgumentParser.h
│   │   ├── ArgumentParser.cpp
│   │   └── Option.h
│   │
│   └── prompt/
│       ├── Prompt.h
│       ├── Prompt.cpp
│       └── Style.h
│
├── commands/
│   ├── init/
│   │   ├── InitCommand.h
│   │   └── InitCommand.cpp
│   │
│   └── add/
│       ├── AddCommand.h
│       └── AddCommand.cpp
│
├── config/
├── filesystem/
├── jdk/
└── maven/
```

Additional modules will be introduced as their responsibilities become necessary.

---

# CLI layer

The CLI layer handles interaction with the terminal.

It should not contain Maven-specific business logic.

## ArgumentParser

Responsible for parsing:

```bash
mvnx init my-app --java 21
```

into:

```text
command arguments
options
flags
```

Examples:

```text
my-app      positional argument
--java 21   option
-j 21       short option
```

It should eventually support:

- positional arguments
- long options
- short options
- boolean flags
- required option values
- useful parsing errors
- `--option=value`
- `--` argument termination

---

# Prompt

`Prompt` abstracts interactive terminal input.

Commands should not directly depend on the line-editing implementation.

Desired API:

```cpp
Prompt prompt;

auto name = prompt.text(
    "Project name",
    "my-project"
);
```

Future:

```cpp
auto type = prompt.select(
    "Project type",
    {
        "Java",
        "Spring Boot",
        "Library",
        "CLI"
    }
);
```

The underlying implementation may use `replxx`, but command code should not need to know this.

This allows terminal behavior to evolve independently.

---

# Style

Terminal styling should be centralized.

Example:

```cpp
Style::CYAN
Style::GREEN
Style::RED
Style::DIM
Style::BOLD
Style::RESET
```

Commands should avoid scattering raw ANSI sequences such as:

```cpp
"\033[36m"
```

throughout the codebase.

A future implementation may detect terminal capabilities and disable styling automatically when appropriate.

---

# Commands

Commands are entry points for user operations.

Examples:

```text
InitCommand
AddCommand
RemoveCommand
InstallCommand
RunCommand
```

A command should coordinate domain services rather than implement all functionality itself.

For example, `AddCommand` should eventually coordinate:

```text
ArgumentParser
      │
      ▼
DependencyParser
      │
      ▼
DependencyResolver
      │
      ▼
PomFile
```

---

# Project generation

Project initialization should eventually be separated from `InitCommand`.

Proposed:

```text
project/
├── ProjectConfig.h
├── ProjectType.h
│
├── generator/
│   ├── ProjectGenerator.h
│   ├── JavaProjectGenerator.h
│   ├── JavaProjectGenerator.cpp
│   ├── SpringProjectGenerator.h
│   └── SpringProjectGenerator.cpp
│
└── validation/
    ├── ProjectValidator.h
    └── ProjectValidator.cpp
```

Example configuration:

```cpp
struct ProjectConfig
{
    std::string name;
    std::string groupId;
    std::string javaVersion;
    ProjectType type;
};
```

`InitCommand` should collect configuration.

The generator should create the project.

The validator should determine whether the configuration is valid.

---

# Transactional project generation

Project generation should avoid leaving partially-created projects behind.

Potential flow:

```text
validate
   │
   ▼
create project directory
   │
   ▼
generate files
   │
   ├── success ──► keep project
   │
   └── failure ──► remove files created by mvnx
```

Rollback must only delete a directory known to have been created by the current `mvnx` operation.

Existing user directories must never be deleted as part of rollback.

---

# Dependency domain

Dependency input should be represented independently from Maven Central or XML.

Proposed:

```text
dependency/
├── DependencyRequest.h
├── DependencyParser.h
├── DependencyParser.cpp
├── DependencyResolver.h
├── DependencyResolver.cpp
└── ResolvedDependency.h
```

## DependencyRequest

Represents what the developer requested.

Example:

```cpp
struct DependencyRequest
{
    std::string groupId;
    std::string artifactId;
    std::string version;
};
```

Examples:

```text
lombok
        ↓
{ "", "lombok", "" }

org.projectlombok:lombok
        ↓
{ "org.projectlombok", "lombok", "" }
```

---

# DependencyParser

Responsible only for interpreting dependency expressions.

It should not:

- make HTTP requests
- search Maven Central
- modify the POM

Examples:

```text
lombok

lombok:1.18.48

org.projectlombok:lombok

org.projectlombok:lombok:1.18.48
```

Two-component expressions require a clearly documented grammar because:

```text
foo:bar
```

could theoretically mean:

```text
artifact:version
```

or:

```text
group:artifact
```

The parser must follow deterministic rules rather than guess unpredictably.

---

# DependencyResolver

Transforms a `DependencyRequest` into exact Maven coordinates.

Example:

```text
DependencyRequest
{ "", "lombok", "" }

        │
        ▼

DependencyResolver

        │
        ▼

ResolvedDependency
{
    "org.projectlombok",
    "lombok",
    "1.18.48"
}
```

The resolver coordinates discovery and metadata services.

---

# ResolvedDependency

Represents an exact dependency:

```cpp
struct ResolvedDependency
{
    std::string groupId;
    std::string artifactId;
    std::string version;
};
```

A `ResolvedDependency` should contain enough information to safely modify a POM.

---

# Maven integration

Maven-specific behavior belongs under:

```text
maven/
```

Potential structure:

```text
maven/
├── repository/
│   ├── MavenMetadata.h
│   ├── MavenMetadataClient.h
│   ├── MavenMetadataClient.cpp
│   ├── MavenCentralSearchProvider.h
│   └── MavenCentralSearchProvider.cpp
│
└── pom/
    ├── PomFile.h
    └── PomFile.cpp
```

---

# Maven Central search

Search answers:

> Which artifact does this human-friendly name refer to?

Example:

```text
guice
   │
   ▼
MavenCentralSearchProvider
   │
   ▼
[
    com.google.inject:guice,
    io.github.replay-framework:guice,
    ...
]
```

Search results should not automatically be treated as exact dependencies.

Ambiguous results require ranking or user selection.

---

# Maven metadata

Metadata answers:

> What versions exist for these exact coordinates?

Example:

```text
org.projectlombok:lombok
          │
          ▼
MavenMetadataClient
          │
          ▼
maven-metadata.xml
          │
          ▼
MavenMetadata
```

Possible representation:

```cpp
struct MavenMetadata
{
    std::string latest;
    std::string release;
    std::vector<std::string> versions;
};
```

Search and metadata are intentionally separate responsibilities.

---

# HTTP

Network access should be abstracted.

Potential API:

```cpp
struct HttpResponse
{
    int statusCode;
    std::string body;
};

class HttpClient
{
public:
    HttpResponse get(const std::string &url);
};
```

HTTP consumers should not need to directly configure the underlying HTTP library.

The implementation should eventually handle:

- connection timeout
- request timeout
- HTTP status errors
- TLS errors
- unavailable services
- useful error messages

---

# POM handling

`PomFile` should provide structural Maven POM operations.

Example:

```cpp
PomFile pom("pom.xml");

pom.addDependency(dependency);
pom.save();
```

Potential operations:

```cpp
bool hasDependency(...);

void addDependency(...);

void removeDependency(...);

void updateDependency(...);
```

XML parsing should use a real XML parser.

Manual string insertion should not be used for existing POM files.

---

# Multiple dependency operations

Commands should avoid partially modifying projects.

For:

```bash
mvnx add lombok guice postgresql
```

desired flow:

```text
parse all
    │
resolve all
    │
validate all
    │
modify in memory
    │
save pom.xml once
```

If dependency #3 fails to resolve, dependencies #1 and #2 should not already have been permanently added.

---

# JDK

JDK functionality should live independently under:

```text
jdk/
```

Possible future components:

```text
JdkDetector
JdkInstaller
JdkManager
JavaVersion
```

This allows:

```bash
mvnx java list
mvnx java install 21
mvnx java use 21
```

without coupling JDK management to `InitCommand`.

---

# Configuration

Future `mvnx` configuration belongs under:

```text
config/
```

Possible configuration sources:

```text
mvnx.toml
global configuration
environment variables
CLI options
```

Precedence should eventually be explicitly defined.

---

# Filesystem

Reusable filesystem operations should move to:

```text
filesystem/
```

when abstractions become useful.

This may include:

- safe directory creation
- temporary files
- atomic writes
- rollback
- executable discovery
- user directories
- cache directories

The goal is not to wrap every `std::filesystem` operation unnecessarily.

Create abstractions only where `mvnx` needs additional behavior.

---

# Error handling

Errors should eventually be categorized rather than represented only by arbitrary strings.

Potential categories:

```text
CliError
ValidationError
FilesystemError
NetworkError
MavenError
DependencyResolutionError
PomError
JdkError
```

Terminal presentation should happen near the CLI boundary.

Lower-level services should report useful structured errors without deciding how they should visually appear.

---

# Dependency direction

A useful architectural rule is:

```text
CLI
 ↓
Commands
 ↓
Domain/services
 ↓
Infrastructure
```

Infrastructure should not depend on commands.

For example:

```text
MavenMetadataClient
```

must not know anything about:

```text
AddCommand
```

while `AddCommand` may use `MavenMetadataClient` indirectly through dependency-resolution services.

---

# Testing strategy

Logic should be designed so it can be tested without requiring interactive terminal input.

Important initial test targets:

```text
ArgumentParser
ProjectValidator
DependencyParser
DependencyResolver
PomFile
Maven metadata parsing
```

Network-facing components should eventually be testable independently from the real Maven Central service.

Filesystem generation should be testable using temporary directories.

---

# Architectural principle

When deciding where new functionality belongs, ask:

> Does this code understand the CLI, the domain operation, Maven, the filesystem, or the network?

It should normally belong to only one of those areas.

The goal is not to maximize the number of classes.

The goal is to keep responsibilities clear enough that `mvnx` can grow without every new command becoming a giant source file.