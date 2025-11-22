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

} // namespace O::Configuration::Module

#endif //CONFIGURATION_MODULE_TRAITS_H