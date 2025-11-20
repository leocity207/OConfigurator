#ifndef CONFIGURATION_APPLICATION_JSON_WRITER_H
#define CONFIGURATION_APPLICATION_JSON_WRITER_H

#include <tuple>
#include <filesystem>
#include "include/expected.h"


#include "container.h"

namespace O::Configuration::Application
{
	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_File(std::filesystem::path& path);

	template<class... Data_Modules>
	Expected_Builder<Data_Modules...> Build_From_JSON_String(std::string_view data);

} // namespace O::Configuration::Application

#include "json_builder.hpp"

#endif //CONFIGURATION_APPLICATION_JSON_WRITER_H
