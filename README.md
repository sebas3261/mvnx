# mvnx

**A modern developer experience for Maven.**

`mvnx` is an open-source native CLI that brings a simpler, more interactive, npm-like developer experience to Maven projects — without replacing Maven.

```bash
mvnx init my-app
cd my-app

# Planned
mvnx add lombok
mvnx add org.postgresql:postgresql
```

> **mvnx enhances Maven. It doesn't replace it.**

Projects created or modified with `mvnx` remain standard Maven projects.

---

## Why mvnx?

Maven is powerful, mature, and widely used throughout the Java ecosystem.

However, many everyday operations still involve verbose commands, manual `pom.xml` editing, or knowledge of exact Maven coordinates.

For example, adding a dependency normally requires finding its coordinates and manually editing the POM:

```xml
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.18.48</version>
</dependency>
```

The long-term goal of `mvnx` is to make common workflows simpler:

```bash
mvnx add lombok
```

while still producing a completely standard Maven project.

---

## Design philosophy

### Maven stays Maven

`mvnx` is not a new build system.

It does not replace:

- Maven dependency resolution
- Maven repositories
- Maven lifecycle
- `pom.xml`
- Maven plugins
- existing Maven tooling

Instead, `mvnx` acts as a developer-experience layer around Maven.

The `pom.xml` remains the source of truth.

You should always be able to stop using `mvnx` and continue with:

```bash
mvn compile
mvn test
mvn package
```

without migrating or converting your project.

### No lock-in

A project created with:

```bash
mvnx init my-app
```

should still be a normal Maven project.

A project modified with:

```bash
mvnx add lombok
```

should still contain normal Maven dependency declarations.

`mvnx` should improve the workflow, not create a new ecosystem that users become dependent on.

---

# Current features

## `mvnx init`

Create a new Maven project interactively:

```bash
mvnx init
```

Or provide configuration directly:

```bash
mvnx init my-app --group-id com.example --java 21
```

`mvnx init` currently provides:

- interactive project creation
- editable terminal input
- sensible defaults
- project-name validation
- Java package validation
- Java version validation
- Maven-standard directory structure
- automatic package generation
- `Main.java` generation
- `pom.xml` generation
- protection against existing project directories

Generated structure:

```text
my-app/
├── pom.xml
└── src/
    ├── main/
    │   └── java/
    │       └── com/example/myapp/
    │           └── Main.java
    └── test/
        └── java/
            └── com/example/myapp/
```

The generated project can immediately be built using Maven:

```bash
cd my-app
mvn package
```

---

# Planned commands

The following commands describe the direction of the project and are **not necessarily implemented yet**.

## Dependencies

```bash
mvnx add lombok
mvnx add lombok:1.18.48
mvnx add org.projectlombok:lombok
mvnx add org.projectlombok:lombok:1.18.48

mvnx remove lombok

mvnx update
mvnx outdated
```

Short dependency names should be resolved through Maven Central when possible.

For ambiguous artifacts, `mvnx` should allow the developer to choose:

```text
◇  Multiple artifacts found
│
│  com.google.inject:guice
│  io.github.replay-framework:guice
│  com.codeborne.replay:guice
│
◇  Select dependency
│  › com.google.inject:guice
```

Explicit Maven coordinates should always be supported.

---

## Install

Planned:

```bash
mvnx install
```

`mvnx install` is intended to prepare a project for development using Maven as the underlying engine.

It should not redefine Maven dependency resolution.

It is also intentionally different from:

```bash
mvn install
```

which executes Maven's lifecycle through the `install` phase and installs the built artifact into the local Maven repository.

---

## Scripts

A future `mvnx.toml` may provide convenient project commands:

```toml
[java]
version = 21

[scripts]
dev = "mvn spring-boot:run"
build = "mvn clean package"
test = "mvn test"
start = "java -jar target/app.jar"
```

Allowing:

```bash
mvnx run dev
mvnx run build
mvnx run test
```

---

## Java management

Potential future commands:

```bash
mvnx java install 21
mvnx java use 21
mvnx java list
```

The goal would be to make the JDK required by a project easier to discover and configure.

---

# Roadmap

## Project creation

- [x] `mvnx init`
- [x] Interactive terminal input
- [x] Project defaults
- [x] Java/package validation
- [x] Maven project generation
- [ ] Transactional project generation / rollback
- [ ] Project types
- [ ] Spring Boot template
- [ ] Java library template
- [ ] CLI application template

## Dependencies

- [ ] Dependency expression parser
- [ ] Maven Central search
- [ ] Maven metadata resolution
- [ ] Interactive artifact selection
- [ ] `mvnx add`
- [ ] Explicit dependency versions
- [ ] Multiple dependencies per command
- [ ] Duplicate dependency detection
- [ ] `mvnx remove`
- [ ] `mvnx update`
- [ ] `mvnx outdated`

## Maven integration

- [ ] Structural POM parsing
- [ ] Structural POM modification
- [ ] `dependencyManagement` support
- [ ] Parent POM awareness
- [ ] Multi-module project support

## Workflow

- [ ] `mvnx install`
- [ ] `mvnx.toml`
- [ ] `mvnx run`
- [ ] Project scripts

## Java

- [ ] JDK detection
- [ ] `mvnx java list`
- [ ] `mvnx java use`
- [ ] `mvnx java install`

## Distribution

- [ ] macOS ARM64
- [ ] macOS x86_64
- [ ] Linux x86_64
- [ ] Linux ARM64
- [ ] Windows x86_64
- [ ] GitHub Releases
- [ ] Homebrew
- [ ] WinGet
- [ ] Linux installer

---

# Building from source

### Requirements

- C++20-compatible compiler
- CMake 3.20+
- Git

Clone the repository and build:

```bash
git clone <repository-url>
cd mvnx

cmake -S . -B build
cmake --build build
```

Run:

```bash
./build/mvnx
```

Dependencies required by `mvnx` are managed through CMake where possible.

---

# Technology

`mvnx` is primarily written in **C++20**.

Current and planned technologies include:

- C++20
- CMake
- replxx
- Maven Central APIs
- Maven repository metadata
- XML parsing
- native filesystem APIs

C++ allows `mvnx` to be distributed as a native executable without requiring a JVM merely to run the CLI.

---

# Project status

`mvnx` is currently under active development.

The command surface and configuration formats may change before the first stable release.

---

# License

A permissive open-source license is planned for the public release.

Apache License 2.0 is currently being considered.