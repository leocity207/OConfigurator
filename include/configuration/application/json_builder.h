#ifndef CONFIGURATION_APPLICATION_JSON_BUILDER_H
#define CONFIGURATION_APPLICATION_JSON_BUILDER_H

// STL
#include <tuple>
#include <filesystem>
#include <string_view>

// UTILS
#include <utils/expected.h>

// APPLICATION
#include "container.h"

namespace O::Configuration::Application
{
	/**
	 * @brief High-level parse errors returned by the application builder.
	 *
	 * These indicate problems at the JSON/document level (not module-specific).
	 */
	enum Parse_Error {
		JSON_PARSING_FAILED,        /**< RapidJSON failed to parse the input. */
		FILE_OPENING_FAILED,        /**< The file could not be opened for reading. */
		JSON_ROOT_IS_NOT_AN_OBJECT  /**< The document root must be a JSON object. */
	};

	/**
	 * @brief Description of a module-level build failure.
	 *
	 * When the builder fails due to a module parse error the Error struct indicates the module (module_name) and the numeric error code returned by the module's builder (error_id).
	 * We ensure that the error_ID can be casted back into a Module_Error for more information if needed.
	 */
	struct Error {
		std::string_view module_name; /**< Name/key of the module that failed (empty for document-level errors). */
		int error_id;                 /**< Numeric error code: either Parse_Error or module-specific Error enumerator cast to int. */
	};

	/**
	 * @brief Alias describing the expected return type of Build_From_JSON_* functions.
	 *
	 * This is an alias for O::Expected<Container<Data_Modules...>, Error>.
	 * On success the Expected contains the assembled Container; on failure it contains an Error describing the failure.
	 */
	template<class... Data_Modules>
	using Expected_Builder = O::Expected<O::Configuration::Application::Container<Data_Modules...>, Error>;

	/**
	 * @brief Build the application Container from a JSON file on disk.
	 *
	 * @tparam Data_Modules List of module data types to include in the container.
	 * @param path Path to the JSON file to parse.
	 * @return Expected_Builder<Data_Modules...> - On success contains the container.
	 *         On error contains Error (module name and error id).
	 */
	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_File(const std::filesystem::path& path);

	/**
	 * @brief Build the application Container from an in-memory JSON string.
	 *
	 * @tparam Data_Modules List of module data types to include in the container.
	 * @param data JSON text to parse.
	 * @return Expected_Builder<Data_Modules...> - On success contains the container.
	 *         On error contains Error (module name and error id).
	 */
	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_String(std::string_view data);
} // namespace O::Configuration::Application

#include "json_builder.hpp"

#endif //CONFIGURATION_APPLICATION_JSON_BUILDER_H
