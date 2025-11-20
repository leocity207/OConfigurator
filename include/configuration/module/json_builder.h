#ifndef CONFIGURATION_MODULE_JSON_BUILDER_H
#define CONFIGURATION_MODULE_JSON_BUILDER_H

#include <string>
#include <utility>
#include <type_traits>
#include <optional>
#include <rapidjson/document.h>

namespace O::Configuration::Module
{
	template<class Derived, class Data, class Error>
	struct JSON_Builder
	{
		static_assert(std::is_enum_v<Error>, "Each module configuration must define: enum class Error { ... };");
		static_assert(std::is_same_v<std::underlying_type_t<Error>, int>, "Module::Error must have an underlying type of int.");

		std::optional<Error> Load_From_Json(const rapidjson::Value& v)
		{
			return static_cast<Derived*>(this)->Load_From_Json(v);
		}

		Data operator*()
		{
			return std::move(data);
		}
		
		static constexpr const char* Key() noexcept 
		{ 
			return Derived::Key();
		}

		Data data;
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_BUILDER_H
