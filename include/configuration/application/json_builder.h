#ifndef CONFIGURATION_APPLICATION_JSON_BUILDER_H
#define CONFIGURATION_APPLICATION_JSON_BUILDER_H

#include <tuple>
#include <filesystem>
#include "include/expected.h"


#include "container.h"

namespace O::Configuration::Application
{
	enum Parse_Error {
		JSON_PARSING_FAILED,
		FILE_OPENING_FAILED,
		JSON_ROOT_IS_NOT_AN_OBJECT
	};

	struct Error {
		std::string_view module_name;
		int error_id;
	};

	template<class... Data_Modules>
	using Expected_Builder = O::Expected<O::Configuration::Application::Container<Data_Modules...>, Error>;

	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_File(std::filesystem::path& path);

	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_String(std::string_view data);
} // namespace O::Configuration::Application

#include "json_builder.hpp"

#endif //CONFIGURATION_APPLICATION_JSON_BUILDER_H
