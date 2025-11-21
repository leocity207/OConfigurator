Application object & class hierarchy
===================================

Overview
--------
This page explains the relationships between the main utilities and the
module/application templates in the configuration system:

* `O::Expected<T,E>` — result container for success (value) or failure (error).
* `O::For_Each_In_Tuple` — tuple iteration helper used when walking module data.
* `O::Configuration::Module::Traits<Data>` — specialization point linking a module's
  Data type to its Builder and Writer.
* `O::Configuration::Module::JSON_Builder` (CRTP) — implement `Load_From_Json` and
  `Key()` to parse module JSON into a `Data` instance.
* `O::Configuration::Module::JSON_Writer` (CRTP) — implement `To_JSON` and `Key()`
  to serialize module `Data`.
* `O::Configuration::Application::Container<...>` — tuple holding module data.
* Application builder/writer entry points — `Build_From_JSON_*` and `Write_As_JSON_*`.

Graph (Graphviz)
----------------
.. graphviz::
   :caption: Relation graph between utils, module types and application helpers
   :align: center

   digraph Configurator {
     rankdir=LR;
     node [shape=record,fontsize=10];

     Expected [label="{O::Expected<T,E>|template class\\n- holds Value or Error}", shape=record];
     ForEach [label="O::For_Each_In_Tuple(fn)", shape=oval];
     Container [label="{O::Configuration::Application::Container\\nmodules : tuple<Data...>}", shape=record];
     ExpectedBuilder [label="Expected_Builder<...>\\n(alias = O::Expected<Container, Error>)", shape=note];
     AppError [label="{O::Configuration::Application::Error|module_name: string_view\\nerror_id: int}", shape=record];
     ParseError [label="Parse_Error (enum)", shape=box3d];
     WriteError [label="Write_Error (enum)", shape=box3d];
     BuildFile [label="Build_From_JSON_File(path)()", shape=component];
     BuildString [label="Build_From_JSON_String(data)()", shape=component];
     BuildDoc [label="Build_From_Json_Document(doc)()", shape=component];
     WriteFile [label="Write_As_JSON_File(...)", shape=component];
     WriteString [label="Write_As_JSON_String(...)", shape=component];

     ModuleTraits [label="O::Configuration::Module::Traits<Data>\\n(specialize per Data)", shape=record];
     JSONBuilder [label="JSON_Builder<Derived, Data, Error>\\n- Load_From_Json(v)\\n- Key()", shape=record];
     JSONWriter [label="JSON_Writer<Derived, Data>\\n- To_JSON(writer,data)\\n- Key()", shape=record];
     ModuleData [label="Module Data (user)\\nstruct Data", shape=folder];
     RapidJSON [label="rapidjson::Document / Value / Writer", shape=plaintext];

     /* edges - builder-side */
     BuildFile -> BuildDoc [label="parse file -> doc", fontsize=9];
     BuildString -> BuildDoc [label="parse string -> doc", fontsize=9];
     BuildDoc -> ForEach [label="iterate using", fontsize=9];
     ForEach -> Container [label="iterate modules tuple", fontsize=9];
     BuildDoc -> ModuleTraits [label="query Traits<ModuleType>::Builder", fontsize=9];
     ModuleTraits -> JSONBuilder [label="Builder type", fontsize=9];
     JSONBuilder -> ModuleData [label="produces Data (operator*)", fontsize=9];
     BuildDoc -> JSONBuilder [label="instantiate Builder\ncall Load_From_Json(Value)", fontsize=9];
     BuildDoc -> ExpectedBuilder [label="return success or Error", fontsize=9];
     ExpectedBuilder -> Expected [label="alias -> template", fontsize=9];
     ExpectedBuilder -> AppError [label="contains Error on failure", fontsize=9];
     BuildDoc -> AppError [label="create Error{module_name,error_id}", fontsize=9];

     /* edges - writer-side */
     WriteFile -> ForEach [label="iterate using", fontsize=9];
     WriteString -> ForEach [label="iterate using", fontsize=9];
     ForEach -> ModuleTraits [label="query Traits<ModuleType>::Writer", fontsize=9];
     ModuleTraits -> JSONWriter [label="Writer type", fontsize=9];
     WriteFile -> JSONWriter [label="call To_JSON(writer, data)", fontsize=9];
     WriteString -> JSONWriter [label="call To_JSON(writer, data)", fontsize=9];
     JSONWriter -> RapidJSON [label="writes into rapidjson::Writer", fontsize=9];

     /* data flow between JSON and builder/writer */
     JSONBuilder -> RapidJSON [label="reads from rapidjson::Value", fontsize=9];
   }

How to read the graph
---------------------
* Boxes labeled `JSON_Builder` / `JSON_Writer` represent the CRTP base templates module
  authors derive from.
* `Traits<Data>` points to which concrete `Builder` and `Writer` types are associated
  with the module's `Data` type.
* The application builders (`Build_From_*`) create a RapidJSON `Document` and call
  `Build_From_Json_Document`. That function iterates the application `Container`'s
  tuple with `O::For_Each_In_Tuple`, queries the `Traits` to get a `Builder`, then
  calls `Builder::Load_From_Json` to parse the module's sub-object.
* `Expected_Builder` is an alias over `O::Expected<Container, Error>` and is the
  return type for build operations — it carries a successful `Container` or an
  `Application::Error` with `module_name` and `error_id`.
* The writer functions iterate modules the same way, query `Traits` for the `Writer`
  type and call its `To_JSON` to produce JSON via rapidjson.

Node descriptions
-----------------
* **O::Expected<T,E>** – small, move-only result type used for build errors or results.
* **O::For_Each_In_Tuple** – zero-overhead helper to apply a callable to every tuple element.
* **Traits<Data>** – specialization point mapping a `Data` type to `Builder` / `Writer`.
* **JSON_Builder** – CRTP base that requires derived builder to implement `Load_From_Json` and `Key`.
* **JSON_Writer** – CRTP base that requires derived writer to implement `To_JSON` and `Key`.
* **Container** – application-level tuple of module data types.
* **Build / Write functions** – high-level entry points that glue everything together.

Common flows (summary)
---------------------
1. **Parsing a file**: `Build_From_JSON_File` → parse to `rapidjson::Document` → `Build_From_Json_Document` → iterate modules → for each module: get Builder via `Traits`, call `Load_From_Json`, on success move data into `Container`, on failure return `Expected_Builder` with `Error`.
2. **Serializing**: `Write_As_JSON_String/File` → iterate modules → for each module: get Writer via `Traits`, call `To_JSON` to emit JSON.

Notes & recommendations
-----------------------
* Keep module `Key()` stable — it is the JSON key used by both parser and writer.
* The `Error` object returned by builders is a numeric `error_id` to remain lightweight; prefer enumerations with explicit underlying `int`.
* `O::For_Each_In_Tuple` removes index-sequence boilerplate and preserves compile-time inlining.