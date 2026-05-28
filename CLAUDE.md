# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build

Dependencies are managed by vcpkg. Set `VCPKG_ROOT` before building.

```sh
vcpkg install           # install/update deps from vcpkg.json
./muerta.sh             # clean build + run (rm -rf build && cmake + make)
```

Manual build:
```sh
mkdir build && cd build && cmake .. && make && ./muerta_<target>
./build/muerta_<target>
```

Adding a new dependency: `vcpkg add port <name>`, then `vcpkg install`.

## Architecture

The app follows a strict layered architecture wired together by a manual DI container:

```
src/apps/cli/main.cpp          Entry point — creates Container, uses services
src/core/container/            DI container (Container) owns all heap resources
src/core/infra/sqlite.hpp      SQLiteDB wrapper — opens ./muerta.db relative to CWD
src/core/repository/todo/      TodosRepository — raw CRUD via sqlite3 API
src/core/services/todo/        TodoService — business logic on top of repository
src/core/helper/tz.hpp         Timestamp conversion (chrono ↔ string)
```

**Dependency flow**: `SQLiteDB` → `TodosRepository` → `TodoService` → CLI

`Container` owns all objects as `std::unique_ptr` and wires them via constructor injection (refs, not pointers, at the boundary). It throws `std::runtime_error` on DB open failure. Only `Container` is created in `main`.

## Conventions

- Use `.hpp` for all headers (not `.h`).
- All warnings are enabled and treated as errors (`-Wall -Wextra -Wpedantic -Werror` + many more). Clang is the preferred compiler.
- UUIDs use `uuid_t` (POSIX libuuid); parse with `uuid_parse`, check with `uuid_is_null`.
- Timestamps stored/retrieved as strings via `toTimestamp`/`fromTimestamp` in `src/core/helper/tz.hpp`.
- SQLite statements use `sqlite3_prepare_v2` + RAII `unique_ptr` with a custom deleter for `sqlite3_stmt`.
- Log with `spdlog::info/error` — no `std::cout`.
