# Muerta Todo List App

This README will guide you through setting up and running the project using vcpkg and CMake.

## Prerequisites

- C++ compiler (e.g., GCC, Clang) i'd like to preferred to clang
- C++ version 23 or higher
- CMake (>= 4.1 recommended)
- Git
- vcpkg

## 1. Install vcpkg dependencies

If you haven't already, install vcpkg:

```sh
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.sh
```

Install dependencies for this project:

```sh
cd muerta
vcpkg install
```

## 2. Configure and build the project

Use CMake to configure, build and run (for example debug profile):

```sh
./muerta.sh
```

or see `muerta.sh` file to edit to your entry point

default for cli and add params `./muerta.sh gui` to build gui version (incoming)

## 3. Run the project

After build with bash script it will automatically running, if it not try to run manual the executable:

```sh
./build/muerta_cli
```

## 4. Notes

- The project uses dependencies managed by vcpkg (e.g., sqlite3, spdlog, uuid).
- If you add new dependencies, try to run `vcpkg add port <deps_name_1> <deps_name_2> <deps_name_3>`, it will be automatically update `vcpkg.json` and then run manually `vcpkg install` again.
- For IDE integration (e.g., CLion), set the CMake toolchain file to vcpkg's in your IDE settings.

## 5. Troubleshooting

- If you see errors about missing packages, ensure your vcpkg is up to date and the toolchain file is set correctly.
- For CMake errors about missing `unofficial-sqlite3`, check that vcpkg installed it and that your toolchain file is used.
