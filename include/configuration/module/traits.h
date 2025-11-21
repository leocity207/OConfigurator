/**
 * @file traits.h
 * @brief Traits specialization point to connect Data types with Builders/Writers.
 *
 * Projects using the configuration subsystem must provide specializations of
 * `O::Configuration::Module::Traits<Data>` for each module data type. Each
 * specialization must expose (at minimum) nested aliases named `Builder` and
 * `Writer` which refer to the module's builder and writer types respectively.
 *
 * Example specialization:
 * @code
 * namespace O::Configuration::Module {
 *     template<>
 *     struct Traits<MyModuleData>
 *     {
 *         using Builder = MyModule::Builder; // builder type that produces MyModuleData
 *         using Writer  = MyModule::Writer;  // writer type that serializes MyModuleData
 *     };
 * }
 * @endcode
 */

#ifndef CONFIGURATION_MODULE_TRAITS_H
#define CONFIGURATION_MODULE_TRAITS_H

namespace O::Configuration::Module
{

	/**
	 * @brief Specialize this trait for each module data type.
	 *
	 * @tparam Data The concrete module data structure type.
	 *
	 * The specialization must provide at least these nested type aliases:
	 * - `using Builder = <builder type>`; // builder must be compatible with JSON_Builder
	 * - `using Writer  = <writer type>`;  // writer must be compatible with JSON_Writer
	 *
	 * The application-level code relies on these aliases to obtain the
	 * appropriate parser/serializer for each module stored in the container.
	 */
	template<class Data>
	struct Traits;

}

#endif //CONFIGURATION_MODULE_TRAITS_H