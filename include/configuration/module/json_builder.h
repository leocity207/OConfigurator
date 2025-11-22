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
	 *
	 * `Load_From_JSON` should return an engaged std::optional<Error> on
	 * failure and std::nullopt on success. On success the builder should
	 * populate its `data` member which callers obtain by dereferencing the
	 * builder (operator*).
	 */
	template<class Derived, class Data, class Error>
	struct JSON_Builder
	{
		static_assert(std::is_enum_v<Error>, "Each module configuration must define: enum class Error { ... };");
		static_assert(std::is_same_v<std::underlying_type_t<Error>, int>, "Module::Error must have an underlying type of int.");

		/**
		 * @brief Invoke the concrete builder's Load_From_JSON implementation.
		 *
		 * This wrapper simply forwards to the Derived implementation so the
		 * CRTP-based type has a consistent public surface.
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
		 * Calling code can use `*builder` to obtain the parsed Data once
		 * the `Load_From_JSON` call has succeeded.
		 *
		 * @note The returned Data is moved from the builder.
		 */
		Data operator*()
		{
			return std::move(data);
		}
		
		/**
		 * @brief Return the JSON key used by this module.
		 *
		 * This helper delegates to `Derived::Key()`; module implementers
		 * must provide that static function.
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