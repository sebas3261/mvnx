# mvnx — Vision

## The idea

`mvnx` aims to provide a modern developer experience for Maven.

Maven is not the problem `mvnx` is trying to solve.

Maven provides a mature build lifecycle, dependency resolution system, plugin ecosystem, repository model, and project format used throughout the Java ecosystem.

`mvnx` exists to make interacting with those capabilities simpler.

The guiding idea is:

> **mvnx enhances Maven. It doesn't replace it.**

---

# The problem

Developers coming from ecosystems such as npm, pnpm, Cargo, or modern developer CLIs are accustomed to workflows such as:

```bash
pnpm add axios
cargo add serde
```

Maven frequently requires a more manual workflow.

Adding a dependency may involve:

1. searching Maven Central
2. finding the correct artifact
3. determining the appropriate version
4. copying its Maven coordinates
5. opening `pom.xml`
6. finding or creating `<dependencies>`
7. inserting the dependency
8. returning to the terminal

The underlying Maven dependency model is powerful.

The interaction can be improved.

`mvnx` should make the common case:

```bash
mvnx add lombok
```

---

# Core principles

## 1. Maven remains the engine

`mvnx` should not implement a competing Maven build system.

Where Maven already has mature functionality, `mvnx` should orchestrate or configure Maven instead of reimplementing it.

This includes:

- dependency resolution
- transitive dependencies
- Maven repositories
- mirrors
- credentials
- build lifecycle
- plugins
- BOMs
- packaging

---

## 2. Standard Maven projects

Using `mvnx` must not require adopting a proprietary project format.

The following:

```bash
mvnx init my-app
```

must generate a standard Maven project.

The following:

```bash
mvnx add lombok
```

must modify the Maven project in a standards-compatible way.

At any point, a developer should be able to uninstall `mvnx` and continue using:

```bash
mvn compile
mvn test
mvn package
```

---

## 3. `pom.xml` remains the source of truth

`mvnx` may eventually introduce optional configuration such as:

```text
mvnx.toml
```

but dependency declarations must not be moved away from Maven.

For example, this would be undesirable:

```toml
[dependencies]
lombok = "1.18.48"
```

if it caused `pom.xml` to stop being authoritative.

Maven tools, IDEs, CI systems, and developers should continue understanding the project without requiring `mvnx`.

---

## 4. No unnecessary lock-in

A developer should be able to:

```bash
mvnx init my-api
mvnx add lombok
```

and then never use `mvnx` again.

The project must continue working normally.

This makes trying `mvnx` low-risk.

---

## 5. Optimize common workflows

`mvnx` should not expose every Maven capability through a second syntax.

Commands should exist when they meaningfully improve the developer experience.

Examples:

```bash
mvnx init
mvnx add
mvnx remove
mvnx update
mvnx outdated
mvnx install
mvnx run
```

There is no requirement for every:

```bash
mvn ...
```

command to have an:

```bash
mvnx ...
```

equivalent.

---

# Dependency experience

One of the primary goals of `mvnx` is improving dependency management.

## Short names

Desired:

```bash
mvnx add lombok
```

Possible flow:

```text
lombok
   │
   ▼
Maven Central search
   │
   ▼
org.projectlombok:lombok
   │
   ▼
Maven repository metadata
   │
   ▼
latest stable release
   │
   ▼
pom.xml
```

---

## Explicit versions

```bash
mvnx add lombok:1.18.48
```

should resolve the artifact and validate the requested version.

---

## Maven coordinates

Experienced Maven users should be able to bypass search:

```bash
mvnx add org.projectlombok:lombok
```

or specify everything:

```bash
mvnx add org.projectlombok:lombok:1.18.48
```

The more information the developer supplies, the less discovery `mvnx` should perform.

---

## Ambiguous dependencies

`mvnx` must never silently select an arbitrary artifact simply because it appeared first in a search result.

For:

```bash
mvnx add guice
```

multiple artifacts may exist.

The user should be able to select the intended dependency interactively.

Correctness is more important than pretending the CLI can magically infer every intention.

---

# Dependency resolution

`mvnx` should distinguish between two concepts.

## Artifact discovery

Question:

> Which Maven artifact does "lombok" refer to?

This can use Maven Central search.

## Version metadata

Question:

> Which versions exist for `org.projectlombok:lombok`?

This should use Maven repository metadata where appropriate.

For example:

```text
org.projectlombok:lombok
        │
        ▼
maven-metadata.xml
        │
        ├── latest
        ├── release
        └── versions
```

These responsibilities should remain separate internally.

---

# POM manipulation

`pom.xml` must be treated as XML, not as a string template once modifying existing projects.

`mvnx` should not rely on operations such as:

```text
find "</dependencies>"
insert text before it
```

Real Maven projects may contain:

- parents
- properties
- dependency management
- plugins
- profiles
- repositories
- modules
- inherited configuration
- BOM imports

POM modifications should therefore be structural.

---

# `mvnx install`

`mvnx install` intentionally does not mean exactly the same thing as:

```bash
mvn install
```

In Maven, `install` is a lifecycle phase that installs the project's built artifact into the local Maven repository.

The intended `mvnx install` experience is closer to:

> Prepare this project and its dependencies so I can work on it.

Maven should still perform Maven dependency resolution.

`mvnx` should orchestrate rather than reimplement that process.

Exact behavior will be defined before implementation.

---

# Project scripts

An optional future configuration file may provide project-level developer commands.

Example:

```toml
[java]
version = 21

[scripts]
dev = "mvn spring-boot:run"
build = "mvn clean package"
test = "mvn test"
```

Then:

```bash
mvnx run dev
mvnx run build
mvnx run test
```

This configuration is intended for developer convenience.

It should not replace Maven's POM.

---

# JDK management

A future version may help developers manage Java installations:

```bash
mvnx java install 21
mvnx java use 21
mvnx java list
```

Potential responsibilities include:

- detecting installed JDKs
- determining project Java requirements
- downloading supported JDK distributions
- selecting a JDK for the current project

JDK management should remain modular and optional.

---

# Project initialization

`mvnx init` should evolve into a polished project generator.

Potential project types:

```text
Java
Spring Boot
Library
CLI
```

Interactive configuration should remain simple and terminal-native.

Example:

```text
◆  Create a new Maven project

◇  Project name (my-project)
│  ›

◇  Project type
│  Java

◇  Group ID (com.example)
│  ›

◇  Java version (21)
│  ›

◆  Project created successfully!
```

The CLI should be visually polished without becoming a full-screen TUI.

---

# Terminal experience

The CLI should have a consistent visual language.

Examples:

```text
◆  action / success

◇  question / progress

│  visual flow

›  user input

✖  error
```

Colors should be restrained.

Suggested semantics:

```text
cyan     interaction / primary action
green    success
red      errors
dim      secondary information
bold     important values
```

Terminal styling must never make the CLI difficult to use in:

- CI
- redirected output
- unsupported terminals
- accessibility-oriented environments

A future `--no-color` option should be considered.

---

# Native CLI

`mvnx` is implemented in C++.

The CLI should eventually be distributable as native binaries for:

```text
macOS ARM64
macOS x86_64
Linux x86_64
Linux ARM64
Windows x86_64
```

Running `mvnx` itself should not require a JVM.

A JDK/Maven installation may still be required when the requested operation inherently uses Java or Maven.

---

# Distribution

The long-term installation experience should be simple.

macOS:

```bash
brew install mvnx
```

Windows:

```bash
winget install mvnx
```

Linux may initially provide a portable installer in addition to package-manager options.

GitHub Releases should provide standalone binaries.

Release creation should eventually be automated through CI/CD.

---

# Open source

`mvnx` is intended to be developed openly.

Desired outcomes include:

- external contributors
- issues from real users
- pull requests
- community-created integrations
- package-maintainer contributions
- forks experimenting with new ideas

Forks are a feature of open-source development, not a failure of the original project.

The canonical project should maintain clear authorship, licensing, release history, and project identity.

---

# What mvnx should NOT become

`mvnx` should avoid becoming:

### Another Maven

Do not rebuild Maven's dependency resolver, lifecycle, or plugin system merely to own the implementation.

### A proprietary Maven format

Do not require developers to convert existing Maven projects into an `mvnx`-specific format.

### A giant wrapper

Do not create `mvnx` aliases for every Maven command unless there is meaningful UX improvement.

### Magic that sacrifices correctness

Do not silently guess dependencies, versions, modules, or project configuration when ambiguity could materially change the project.

Ask the developer when necessary.

---

# Success

The project is successful if a Maven developer thinks:

> "I could do this manually with Maven, but mvnx makes it easier."

The ultimate goal is not to make Maven disappear.

It is to make working with Maven feel better.