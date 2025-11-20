#ifndef CONFIGURATION_APPLICATION_BUILDER_H
#define CONFIGURATION_APPLICATION_BUILDER_H

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

	template<class... DataModules>
	using Expected_Builder = O::Expected<O::Configuration::Application::Container<DataModules...>, Error>;

	template<class... DataModules>
	Expected_Builder<DataModules...> Build_From_File(std::filesystem::path& path);

	template<class... DataModules>
	Expected_Builder<DataModules...> Build_From_String(std::string_view data);
} // namespace O::Configuration::Application

#endif //CONFIGURATION_APPLICATION_BUILDER_H
