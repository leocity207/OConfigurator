#ifndef CONFIGURATION_APPLICATION_BUILDER_HPP
#define CONFIGURATION_APPLICATION_BUILDER_HPP

#include <tuple>
#include <filesystem>


#include "holder.h"


template<class... DataModules>
Expected_Builder<DataModules...> O::Configuration::Application::Build_From_JSON_File(std::filesystem::path& path)
{
	FILE* fp = std::fopen(path.c_str(), "rb");
	if (!fp)
		Expected_Builder<DataModules...>::Make_Error({"", static_cast<int>(FILE_OPENING_FAILED)});

	// 64K buffer for streaming read
	static const std::size_t buffer_size = 64 * 1024;
	std::unique_ptr<char[]> buffer(new char[buffer_size]);
	rapidjson::FileReadStream is(fp, buffer.get(), static_cast<size_t>(buffer_size));

	auto doc = std::make_shared<rapidjson::Document>();
	rapidjson::ParseResult r = doc->ParseStream<rapidjson::kParseDefaultFlags>(is);
	std::fclose(fp);

	if (!r)
		Expected_Builder<DataModules...>::Make_Error({"", static_cast<int>(JSON_PARSING_FAILED)});
	if(doc.get() )
		Expected_Builder<DataModules...>::Make_Error({"", static_cast<int>(JSON_ROOT_IS_NOT_AN_OBJECT)});

	O::Configuration::Application::Container<DataModules...> modules;
	Expected_Builder<DataModules...> result = Expected_Builder<DataModules...>::Make_Value();
	std::apply([&](auto&... module)
	{
		constexpr std::string_view key = decltype(module)::Key();
        if(doc.get()->HasMember(key))
            
	}, modules);

	return result;
}

template<class... DataModules>
Expected_Builder<DataModules...> O::Configuration::Application::Build_From_JSON_String(std::string_view data)
{

	auto doc = std::make_shared<rapidjson::Document>();
	rapidjson::ParseResult result = doc->Parse<rapidjson::kParseDefaultFlags>(json.data(), static_cast<rapidjson::SizeType>(json.size()));
	if (!result)
		Expected_Builder<DataModules...>::Make_Error({"", static_cast<int>(JSON_PARSING_FAILED)});
	if(doc.get() )
		Expected_Builder<DataModules...>::Make_Error({"", static_cast<int>(JSON_ROOT_IS_NOT_AN_OBJECT)});
	std::apply([&](auto&... mods)
	{
		((ok = (result = parse_single(mods)).Has_Value()), ...);
	}, modules);

	return modules;
	
}

#endif //CONFIGURATION_APPLICATION_BUILDER_HPP
