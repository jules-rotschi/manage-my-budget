# Manage My Budget

Manage My Budget is an application that allows users to sort their bank operations by category (users create their own categories) and to see reviews so they can optimize the usage of their money.

## Features

- Add bank operations to bank accounts
- Categorize bank operations
- Manage budget by category
- See monthly/yearly reviews

## Getting started

### Qt installation

1. If Qt is not installed on your system, install [Qt](https://www.qt.io/).
2. Adapt `CMakeLists.txt` 8th line with path to your Qt directory (`set(Qt6_DIR path/to/Qt/version/compiler/lib/cmake/Qt6)`).

### Build the project with Visual Studio

1. Install "Qt Visual Studio Tools" extension.
2. In the extension menu, set the path to your Qt directory.
3. Open the solution file from this repository in Visual Studio.
4. Build inside Visual Studio.

### Build the project with another build system

In the project directory :

1. Generate a build file : `cmake -G "Your generator" -B /path/to/build-dir` (the build file depends on your build system).
2. Build : `cmake --build /path/to/build-dir`.

## Deployment

1. Generate a Ninja file : `cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -B build`.
2. Build with ninja : `ninja -C build`.
3. Install application on your system : `cmake --install build` (needs to be ran as administrator).
    The installation directory is created in the user executables directory.
4. You can use the installation directory to install ManageMyBudget on another computer that does not have Qt installed.
