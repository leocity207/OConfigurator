#ifndef CONFIGURATION_APPLICATION_JSON_WRITER_H
#define CONFIGURATION_APPLICATION_JSON_WRITER_H

#include <tuple>
#include <filesystem>
#include <optional>
#include "include/expected.h"


#include "container.h"

namespace O::Configuration::Application
{
	enum class Write_Error {
		FILE_OPEN_FAILED,
		FILE_WRITE_FAILED
	};

	template<class... Data_Modules>
	std::optional<Write_Error> Write_As_JSON_File(const Container<Data_Modules...>& data, const std::filesystem::path& filepath);

	template<class... Data_Modules>
	std::string Write_As_JSON_String(const O::Configuration::Application::Container<Data_Modules...>& datas);

} // namespace O::Configuration::Application

#include "json_writer.hpp"

#endif //CONFIGURATION_APPLICATION_JSON_WRITER_H
