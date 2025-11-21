Application namespace (O::Configuration::Application)
====================================================

Overview
--------
This page documents the application-level helpers used to build and write the
configuration container composed of per-module data types.

The application helpers:
- Compose module builders and writers into a single Container.
- Provide convenience functions to parse a JSON file/string into the Container.
- Provide convenience functions to serialize the Container to a string or file.

.. contents::
   :local:
   :depth: 2

Container
---------
Short description
^^^^^^^^^^^^^^^^^
A small, type-safe heterogeneous container for module data. The container's
tuple is public and a convenience `Get<T>()` provides typed access.

.. doxygentypedef:: O::Configuration::Application::Container<DataModules...>
    :project: your_project_name
    :members:
    :protected-members:

.. doxygenclass:: O::Configuration::Application::Container<DataModules...>
    :members:
    :protected-members:

JSON Builder (Build_From_JSON_*)
-------------------------------
Short description
^^^^^^^^^^^^^^^^^
Application-level entry points for parsing JSON into the Container. The builder
walks the document and delegates each module's JSON sub-object to the module's
Builder (obtained from Module::Traits<Data>::Builder).

.. doxygenenum:: O::Configuration::Application::Parse_Error
    :members:

.. doxygenclass:: O::Configuration::Application::Error
    :members:
    :protected-members:

.. doxygentypedef:: O::Configuration::Application::Expected_Builder<Data_Modules...>
    :members:

.. doxygenfunction:: O::Configuration::Application::Build_From_JSON_File
    :project: your_project_name

.. doxygenfunction:: O::Configuration::Application::Build_From_JSON_String
    :project: your_project_name

Example
^^^^^^^
.. code-block:: cpp

    using AppContainer = O::Configuration::Application::Container<MyModule1Data, MyModule2Data>;
    std::filesystem::path p = "config.json";
    auto res = O::Configuration::Application::Build_From_JSON_File<MyModule1Data, MyModule2Data>(p);

    if (res) {
      auto container = std::move(res).Value();
      // success
    } else {
      auto err = std::move(res).Error();
      // err.module_name / err.error_id
    }

JSON Writer (Write_As_JSON_*)
-----------------------------
Short description
^^^^^^^^^^^^^^^^^
Helpers to serialize the application Container to a JSON string or write it to disk.
Writers for each module are obtained from Module::Traits<Data>::Writer.

.. doxygenenum:: O::Configuration::Application::Write_Error
    :members:

.. doxygenfunction:: O::Configuration::Application::Write_As_JSON_File
    :project: your_project_name

.. doxygenfunction:: O::Configuration::Application::Write_As_JSON_String
    :project: your_project_name

Notes
-----
- Builders return module-specific error enumerators (converted to int) or
  application Parse_Error values; the Error struct unifies them.
- Keep the module `Key()` stable—the string is the JSON key used by both
  parser and writer.
- Use `Expected_Builder` to propagate module parse errors in a single type.