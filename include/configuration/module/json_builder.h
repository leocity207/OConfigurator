#ifndef CONFIGURATION_MODULE_JSON_BUILDER_H
#define CONFIGURATION_MODULE_JSON_BUILDER_H

// STL
#include <string>
#include <utility>
#include <type_traits>
#include <optional>

// RAPIDJSON
#include <rapidjson/document.h>

namespace O::Configuration::Module
{
	/**
	 * @brief CRTP base for per-module JSON builders.
	 *
	 * @tparam Derived The concrete builder type implementing the parsing.
	 * @tparam Data    The module's configuration data structure (movable).
	 * @tparam Error   An enumeration type (underlying type must be int) describing parse errors.
	 *
	 * @details
	 * The Derived type must implement the following API:
	 * @code
	 * std::optional<Error> Load_From_JSON(const rapidjson::Value& v);
	 * static constexpr const char* Key() noexcept; // JSON key for this module
	 * @endcode
	 */
	template<class Derived, class Data, class Error>
	struct JSON_Builder
	{
		static_assert(std::is_enum_v<Error>, "Each module configuration must define: enum class Error { ... };");
		static_assert(std::is_same_v<std::underlying_type_t<Error>, int>, "Module::Error must have an underlying type of int.");

		/**
		 * @brief Invoke the concrete builder's Load_From_JSON implementation.
		 *
		 * From this function you should be able to parse all the needed infomration for your configuration.
		 * "rapidjson::Value& v" is already inside the object from key returned by "Key()"
		 *
		 * @param v RapidJSON value to parse.
		 * @return std::optional<Error> engaged on error, std::nullopt on success.
		 */
		std::optional<Error> Load_From_JSON(const rapidjson::Value& v)
		{
			return static_cast<Derived*>(this)->Load_From_JSON(v);
		}

		/**
		 * @brief Move-out the parsed Data object.
		 *
		 * Function used inside the Application Parser to move the data from the json builder to the application
		 *
		 * @note The returned Data is moved from the builder.
		 */
		Data&& operator*()
		{
			return std::move(data);
		}
		
		/**
		 * @brief Return the JSON key used by this module.
		 *
		 * The value returned by Key must be the Key found inside the json file
		 */
		static constexpr const char* Key() noexcept
		{
			return Derived::Key();
		}

		/**
		 * @brief Storage for the parsed data.
		 *
		 * The concrete builder populates this member on successful parsing.
		 */
		Data data;
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_BUILDER_H