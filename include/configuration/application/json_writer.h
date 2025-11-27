#ifndef CONFIGURATION_APPLICATION_JSON_WRITER_H
#define CONFIGURATION_APPLICATION_JSON_WRITER_H

// STL
#include <tuple>
#include <filesystem>
#include <optional>

// UTILS
#include <utils/expected.h>

// APPLICATION
#include "container.h"

namespace O::Configuration::Application
{
	/**
	 * @brief Errors returned when writing configuration to a file.
	 *
	 * FILE_OPEN_FAILED - Could not open target file for writing.
	 * FILE_WRITE_FAILED - Generic failure writing to the file (disk full, etc).
	 */
	enum class Write_Error {
		FILE_OPEN_FAILED,
		FILE_WRITE_FAILED
	};

	/**
	 * @brief Write a container to a JSON file.
	 *
	 * Iterates the container's modules and invokes each module's Writer to serialize its data. 
	 * Returns std::nullopt on success or a Write_Error on failure.
	 *
	 * @tparam Data_Modules module data types in the container.
	 * @param data the container to serialize.
	 * @param filepath the destination filesystem path.
	 * @return std::optional<Write_Error> - std::nullopt on success, otherwise the error.
	 */
	template<class... Data_Modules>
	std::optional<Write_Error> Write_As_JSON_File(const Container<Data_Modules...>& data, const std::filesystem::path& filepath);

	/**
	 * @brief Serialize a container to an in-memory JSON string.
	 *
	 * Uses module writers obtained via Module::Traits to produce a JSON string.
	 *
	 * @tparam Data_Modules module data types in the final application container.
	 * @param datas The container to serialize.
	 * @return std::string The produced JSON document (UTF-8).
	 */
	template<class... Data_Modules>
	std::string Write_As_JSON_String(const O::Configuration::Application::Container<Data_Modules...>& datas);

} // namespace O::Configuration::Application

#include "json_writer.hpp"

#endif //CONFIGURATION_APPLICATION_JSON_WRITER_H
