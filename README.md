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

- Header-only, modern C++ 20 (templates + CRTP).
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

```

And then you can do easy parsing

```C++

// Declare your struct conf
struct Window_Data {
    int width  = 800;
    int height = 600;
};

// Create your builder
struct Window_Builder : O::Configuration::Module::JSON_Builder<Window_Builder, Window_Data, Window_Builder::Error>
{
    enum class Error {
        MISSING_WIDTH,
        MISSING_HEIGHT
    };

    static constexpr const char* Key() noexcept { return "window"; }

    std::optional<Error> Load_From_JSON(const rapidjson::Value& v)
    {
        if (!v.IsObject()) return Error::MISSING_WIDTH;

        if (!v.HasMember("width"))  return Error::MISSING_WIDTH;
        if (!v.HasMember("height")) return Error::MISSING_HEIGHT;

        data.width  = v["width"].GetInt();
        data.height = v["height"].GetInt();
        return std::nullopt;
    }
};

//Create Linker indirection between structure Window_Data <-> Window_Builder
namespace O::Configuration::Module {
    template<>
    struct Traits<Window_Data> {
        using Builder = Window_Builder;
    };
}

//Easy to use
int main()
{
    std::filesystem::path path = "config.json";

    auto result = Build_From_JSON_File<WindowData>(path);

    if (!result) {
        auto err = result.Error();
        std::cerr << "Load failed in module '" << err.module_name << "' error id " << err.error_id << "\n";
        return 1;
    }

    AppConfig config = std::move(result).Value();
}