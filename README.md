![Octoliner Configurator Logo](logo.svg)

# Octoliner Configurator

**A small, header-only C++ configuration framework used by the Octoliner project.**

Octoliner Configurator provides a simple, type-safe way to serialize and
deserialize application configuration made of *modules*. It is intentionally
minimal and designed to be embedded into larger projects (hence the top-level
`O::` namespace used across the codebase).

This library is part of the larger **Octoliner** family of tools (hence the `O` prefix).

---

## Highlights

- Header-only, modern C++ (templates + CRTP).
- Module-based config: each module defines a `Data` structure, a `JSON_Builder` and a `JSON_Writer`.
- Application-level `Container<...>` composes modules into a single object.
- Compact `Expected<T,E>` alternative to exceptions for error propagation.
- Small utilities: `O::For_Each_In_Tuple` for tuple iteration.
- Designed to be easy to fetch & embed via CMake `FetchContent`.

---

## Quick install (CMake + FetchContent)

Drop this into your top-level `CMakeLists.txt`:

```cmake
include(FetchContent)

FetchContent_Declare(
    OConfigurator
    GIT_REPOSITORY https://github.com/leocity207/OConfigurator.git
    UPDATE_DISCONNECTED ON
)

FetchContent_MakeAvailable(OConfigurator)

# then link/include as usual:
# target_include_directories(myexe PRIVATE ${OConfigurator_SOURCE_DIR}/include)
