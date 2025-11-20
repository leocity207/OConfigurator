#ifndef MODULE_CONFIGURATION_H
#define MODULE_CONFIGURATION_H

#include <string>
#include <utility>
#include <type_traits>
#include "expected.h" // your Expected<T,E>
#include <rapidjson/document.h>

namespace O::Configuration::Module
{
	template<class Derived, class Data>
	struct Builder
	{

		static_assert(std::is_base_of_v<Builder<Derived>, Derived>,"module configuration must inherit Configuration::Module<Derived>");
		static_assert(std::is_enum_v<typename Derived::Error>, "Each module configuration must define: enum class Error { ... };");
		static_assert(std::is_same_v<std::underlying_type_t<typename Derived::Error>, int>, "Module::Error must have an underlying type of int.");

		Expected<Data, Derived::Error> Load_From_Json(const rapidjson::Value& v)
		{
			return static_cast<Derived*>(this)->Load_From_Json(v);
		}
		
		static constexpr const char* key() noexcept 
		{ 
			return Derived::Key();
		}
	};

} // namespace O::Configuration::Module

#endif // MODULE_CONFIGURATION_H
