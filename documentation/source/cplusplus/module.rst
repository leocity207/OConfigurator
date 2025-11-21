Module configuration.module
===========================

Module-level overview
---------------------
This page documents the small set of CRTP helpers used to implement per-module
JSON builders and writers and the Traits mapping used by the Application-level
Container. These helpers are intentionally minimal and designed to let each
module focus on parsing/serializing its own fields.

The following templates are documented on this page:

- ``O::Configuration::Module::JSON_Builder`` — CRTP base for module JSON builders.
- ``O::Configuration::Module::JSON_Writer`` — CRTP base for module JSON writers.
- ``O::Configuration::Module::Traits`` — Specialize to connect Data -> Builder/Writer.

.. contents:: Table of contents
   :local:
   :depth: 2

JSON Builder (:cpp:namespace:`O::Configuration::Module`)
-------------------------------------------------------

Short description
^^^^^^^^^^^^^^^^^
A CRTP helper that encapsulates common builder behavior: type checks for the
error enum and thin forwarding to the derived class. The derived builder must
implement:

- ``static constexpr const char* Key() noexcept`` — the JSON key for the module.
- ``std::optional<Error> Load_From_Json(const rapidjson::Value& v)`` — parse
  the module JSON and populate the builder's ``data`` member.


.. doxygenclass:: O::Configuration::Module::JSON_Builder<Derived, Data, Error>
    :members:
    :protected-members:

Example
^^^^^^^
.. code-block:: cpp

    struct MyModuleData { /* ... */ };
    enum class MyError : int { INVALID_FORMAT = 1 };

    struct MyModuleBuilder : O::Configuration::Module::JSON_Builder<MyModuleBuilder, MyModuleData, MyError>
    {
        static constexpr const char* Key() noexcept { return "mymodule"; }

        std::optional<MyError> Load_From_Json(const rapidjson::Value& v) {
            // parse 'v' and fill 'this->data'
            return std::nullopt;
        }
    };

JSON Writer (:cpp:namespace:`O::Configuration::Module`)
------------------------------------------------------

Short description
^^^^^^^^^^^^^^^^^
A CRTP helper for module writers. Derived writers must implement:

- ``static constexpr const char* Key() noexcept`` — JSON key for the module.
- ``template<class RapidJSON_Writer> void To_JSON(RapidJSON_Writer& writer, const Data& data) const``

.. code-block:: cpp

    struct MyModuleData { /* ... */ };
    enum class MyError : int { INVALID_FORMAT = 1 };

    struct MyModuleWriter : O::Configuration::Module::JSON_Builder<MyModuleWriter, MyModuleData>
    {
        static constexpr const char* Key() noexcept { return "mymodule"; }

        template<class jsonWriter>
        void Load_From_Json(const jsonWriter& v,const MyModuleData& data ) {
            v.StartObject();
			v.EndObject();
        }
    };

.. doxygenclass:: O::Configuration::Module::JSON_Writer<Derived, Data>
    :members:
    :protected-members:

Example
^^^^^^^
.. code-block:: cpp

    struct MyModuleWriter : O::Configuration::Module::JSON_Writer<MyModuleWriter, MyModuleData>
    {
        static constexpr const char* Key() noexcept { return "mymodule"; }

        template<class RapidJSON_Writer>
        void To_JSON(RapidJSON_Writer& writer, const MyModuleData& data) const {
            writer.StartObject();
            // write members...
            writer.EndObject();
        }
    };

Traits (:cpp:namespace:`O::Configuration::Module`)
-------------------------------------------------

Short description
^^^^^^^^^^^^^^^^^
Specialize ``O::Configuration::Module::Traits<Data>`` for each module data type to
connect the data type with its corresponding Builder and Writer.

.. doxygenclass:: O::Configuration::Module::Traits<Data>
    :members:
    :protected-members:

Example specialization
^^^^^^^^^^^^^^^^^^^^^^
.. code-block:: cpp

    namespace O::Configuration::Module {
      template<>
      struct Traits<MyModuleData> {
        using Builder = MyModuleBuilder;
        using Writer  = MyModuleWriter;
      };
    }

Notes
-----
- Keep the module Key() strings stable (they become the JSON keys).
- Ensure `Error` enums use `int` as underlying type to avoid ABI surprises.
- Builders should return `std::nullopt` on success and an error enumerator on failure.