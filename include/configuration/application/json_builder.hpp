#ifndef CONFIGURATION_APPLICATION_JSON_BUILDER_HPP
#define CONFIGURATION_APPLICATION_JSON_BUILDER_HPP

// STL
#include <tuple>
#include <filesystem>
#include <cstdio>
#include <memory>

// APPLICATION
#include "container.h"
#include "json_builder.h"

// MODULE
#include <configuration/module/traits.h>

// UTILS
#include <utils/tuple_helper.h>

// RAPIDJSON
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

template<class... Data_Modules>
O::Configuration::Application::Expected_Builder<Data_Modules...> Build_From_JSON_Document(const rapidjson::Document& doc)
{
	using namespace O::Configuration::Application;

	if (!doc.IsObject())
		return Expected_Builder<Data_Modules...>::Make_Error(Error{ "", static_cast<int>(JSON_ROOT_IS_NOT_AN_OBJECT) });

	Expected_Builder<Data_Modules...> result = Expected_Builder<Data_Modules...>::Make_Value();
	Container<Data_Modules...>& container = result.Value();

	bool ok = true;

	O::For_Each_In_Tuple(container.modules, [&](auto& module_part)
		{
			if (!ok) return;

			using ModuleType = std::decay_t<decltype(module_part)>;
			using Builder = typename O::Configuration::Module::Traits<ModuleType>::Builder;

			const char* key = Builder::Key();
			if (!doc.HasMember(key)) return;

			Builder builder;
			auto opt = builder.Load_From_JSON(doc[key]);

			if (opt)
			{
				result = Expected_Builder<Data_Modules...>::Make_Error(Error{ key, static_cast<int>(*opt) });
				ok = false;
				return;
			}

			module_part = std::move(*builder);
		});

	return result;
}

template<class... Data_Modules>
O::Configuration::Application::Expected_Builder<Data_Modules...> O::Configuration::Application::Build_From_JSON_File(const std::filesystem::path& path)
{
	FILE* fp = std::fopen(path.generic_string().c_str(), "rb");
	if (!fp)
		return Expected_Builder<Data_Modules...>::Make_Error(Error{ "", static_cast<int>(FILE_OPENING_FAILED) });

	static const std::size_t buffer_size = 64 * 1024;
	std::unique_ptr<char[]> buffer(new char[buffer_size]);
	rapidjson::FileReadStream is(fp, buffer.get(), buffer_size);

	rapidjson::Document doc;
	rapidjson::ParseResult r = doc.ParseStream<rapidjson::kParseDefaultFlags>(is);

	std::fclose(fp);

	if (!r)
		return Expected_Builder<Data_Modules...>::Make_Error(Error{ "", static_cast<int>(JSON_PARSING_FAILED) });

	return Build_From_JSON_Document<Data_Modules...>(doc);
}


template<class... Data_Modules>
O::Configuration::Application::Expected_Builder<Data_Modules...> O::Configuration::Application::Build_From_JSON_String(std::string_view data)
{
	rapidjson::Document doc;
	rapidjson::ParseResult r =
		doc.Parse<rapidjson::kParseDefaultFlags>(data.data(),
			static_cast<rapidjson::SizeType>(data.size()));

	if (!r)
		return Expected_Builder<Data_Modules...>::Make_Error(
			Error{ "", static_cast<int>(JSON_PARSING_FAILED) });

	return Build_From_JSON_Document<Data_Modules...>(doc);
}


#endif //CONFIGURATION_APPLICATION_JSON_BUILDER_HPP
