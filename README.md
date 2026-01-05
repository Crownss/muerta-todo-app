# Muerta Project

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

Use CMake to configure and build (for example debug profile):

```sh
cmake -B cmake-build-debug -DCMAKE_TOOLCHAIN_FILE="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build cmake-build-debug --target muerta
```
or set env VCPKG_ROOT 

```sh
export VCPKG_ROOT="/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake"
cmake --build cmake-build-debug --target meurta
```

Replace `/path/to/vcpkg` with your actual vcpkg path.

## 3. Run the project

After building, run the executable:

```sh
./cmake-build-debug/muerta
```

## 4. Notes
- The project uses dependencies managed by vcpkg (e.g., sqlite3, spdlog, uuid).
- If you add new dependencies, try to run `vcpkg add port <deps_name_1> <deps_name_2> <deps_name_3>`, it will be automatically update `vcpkg.json` and then run manually `vcpkg install` again.
- For IDE integration (e.g., CLion), set the CMake toolchain file to vcpkg's in your IDE settings.

## 5. Troubleshooting
- If you see errors about missing packages, ensure your vcpkg is up to date and the toolchain file is set correctly.
- For CMake errors about missing `unofficial-sqlite3`, check that vcpkg installed it and that your toolchain file is used.

