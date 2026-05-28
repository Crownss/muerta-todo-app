set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
cmake_minimum_required(VERSION 4.1)
project(muerta)

set(CMAKE_CXX_STANDARD 23)

# Enable all warnings and treat them as errors
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Werror
        -Wconversion
        -Wsign-conversion
        -Wshadow
        -Wnull-dereference
        -Wdouble-promotion
        -Wformat=2
        -Wduplicated-cond
        -Wduplicated-branches
        -Wlogical-op
        -Wuseless-cast
        -Wcast-qual
        -Wcast-align
        -Woverloaded-virtual
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wno-unused-parameter
        -fdiagnostics-color=always
    )
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
endif()

add_executable(muerta_cli src/apps/cli/main.cpp
        src/core/infra/sqlite.hpp
        src/core/infra/sqlite.cpp
        src/core/container/container.cpp
        src/core/container/container.hpp
        src/core/repository/todo/todo.cpp
        src/core/repository/todo/todo.hpp
        src/core/repository/todo/models.hpp
        src/core/helper/tz.cpp
        src/core/helper/tz.hpp
        src/core/services/todo/todo_service.cpp
        src/core/services/todo/todo_service.hpp)

find_package(unofficial-sqlite3 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-libuuid CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
target_link_libraries(muerta_cli PRIVATE unofficial::sqlite3::sqlite3 spdlog::spdlog_header_only unofficial::UUID::uuid imgui::imgui)