Getting Started
===============

This guide introduces the basic workflow for using ``OConfigurator`` in your
project. You will learn:

* How to fetch the library with CMake.
* How to define a configuration module (builder + writer).
* How to register the module with ``Traits``.
* How to build an application configuration from JSON.
* How to save configuration data back to JSON.

The API is entirely header-only and requires no linking.


Installing With CMake FetchContent
----------------------------------

Add the following to your project’s top-level ``CMakeLists.txt``:

.. code-block:: cmake

    include(FetchContent)

    FetchContent_Declare(
        OConfigurator
        GIT_REPOSITORY https://github.com/leocity207/OConfigurator.git
        UPDATE_DISCONNECTED ON
    )

    FetchContent_MakeAvailable(OConfigurator)

After this, include headers normally:

.. code-block:: cmake

    target_include_directories(MyApp PRIVATE ${oconfigurator_SOURCE_DIR}/include)


Defining Your First Module
--------------------------

Each configuration module consists of:

1. A **Data** structure: the values your module stores.
2. A **JSON_Builder**: loads the module from JSON.
3. A **JSON_Writer**: writes the module to JSON.
4. A specialization of ``O::Configuration::Module::Traits`` linking everything.

Example: A simple module storing a window size.

Module Data
^^^^^^^^^^^

.. code-block:: cpp

    struct WindowData {
        int width  = 800;
        int height = 600;
    };

Module Builder
^^^^^^^^^^^^^^

.. code-block:: cpp

    struct WindowBuilder
        : O::Configuration::Module::JSON_Builder<WindowBuilder, WindowData, WindowBuilder::Error>
    {
        enum class Error {
            MISSING_WIDTH,
            MISSING_HEIGHT
        };

        static constexpr const char* Key() noexcept { return "window"; }

        std::optional<Error> Load_From_Json(const rapidjson::Value& v)
        {
            if (!v.IsObject()) return Error::MISSING_WIDTH;

            if (!v.HasMember("width"))  return Error::MISSING_WIDTH;
            if (!v.HasMember("height")) return Error::MISSING_HEIGHT;

            data.width  = v["width"].GetInt();
            data.height = v["height"].GetInt();
            return std::nullopt;
        }
    };

Module Writer
^^^^^^^^^^^^^

.. code-block:: cpp

    struct WindowWriter
        : O::Configuration::Module::JSON_Writer<WindowWriter, WindowData>
    {
        static constexpr const char* Key() noexcept { return "window"; }

        template<class Writer>
        void To_JSON(Writer& writer, const WindowData& d) const
        {
            writer.StartObject();
            writer.Key("width");  writer.Int(d.width);
            writer.Key("height"); writer.Int(d.height);
            writer.EndObject();
        }
    };

Registering Module via Traits
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    namespace O::Configuration::Module {
        template<>
        struct Traits<WindowData> {
            using Builder = WindowBuilder;
            using Writer  = WindowWriter;
        };
    }

Application Configuration
^^^^^^^^^^^^^^^^^^^^^^^^^

To combine multiple modules, create an application container:

.. code-block:: cpp

    using AppConfig = O::Configuration::Application::Container<WindowData>;

You can now build an AppConfig from JSON or produce JSON from it.

Loading Configuration From JSON
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Using a file:

.. code-block:: cpp

    using O::Configuration::Application::Build_From_JSON_File;

    std::filesystem::path path = "config.json";

    auto result = Build_From_JSON_File<WindowData>(path);

    if (!result) {
        auto err = result.Error();
        std::cerr << "Load failed in module '" << err.module_name
                << "' error id " << err.error_id << "\n";
        return 1;
    }

    AppConfig config = std::move(result).Value();

Using a JSON string:

.. code-block:: cpp

    auto result = Build_From_JSON_String<WindowData>(R"({
        "window": { "width": 1024, "height": 768 }
    })");

    if (result) {
        AppConfig config = std::move(result).Value();
    }

Accessing Modules
^^^^^^^^^^^^^^^^^

Use Get<T>() to access module data stored inside the container:

.. code-block:: cpp

    auto& window = config.Get<WindowData>();
    std::cout << "W = " << window.width << " H = " << window.height << "\n";

Writing Configuration Back to JSON
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

To a file:

.. code-block:: cpp

    using O::Configuration::Application::Write_As_JSON_File;

    std::filesystem::path out = "saved.json";
    auto err = Write_As_JSON_File(config, out);

    if (err) {
        std::cerr << "Could not write file\n";
    }

To a string:

.. code-block:: cpp

    using O::Configuration::Application::Write_As_JSON_String;

    std::string json = Write_As_JSON_String(config);
    std::cout << json << "\n";

Full Minimal Example
^^^^^^^^^^^^^^^^^^^^

.. code-block:: cpp

    int main()
    {
        auto result = Build_From_JSON_String<WindowData>(R"({
            "window": { "width": 1280, "height": 720 }
        })");

        if (!result) {
            std::cerr << "Failed to load config\n";
            return 1;
        }

        AppConfig cfg = std::move(result).Value();

        auto& window = cfg.Get<WindowData>();
        window.width = 1920; // modify configuration

        std::string out = Write_As_JSON_String(cfg);
        std::cout << out;
    }

This is the complete lifecycle:
 - Declare your modules.
 - Implement Builder + Writer.
 - Register them using Traits.
 - Build an application container from JSON.
 - Modify the configuration.
 - Write back to JSON.

You are now ready to structure larger configurations using multiple modules.