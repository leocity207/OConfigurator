#ifndef CONFIGURATION_MODULE_JSON_WRITER_H
#define CONFIGURATION_MODULE_JSON_WRITER_H

#include <string>
#include <utility>
#include <type_traits>
#include <rapidjson/document.h>


namespace O::Configuration::Module
{

	template<class Derived, class Data>
	struct JSON_Writer
	{
		static_assert(std::is_base_of_v<Writer<Derived>, Derived>,"module configuration must inherit Configuration::Module<Derived>");

		rapidjson::Value To_Json(rapidjson::Document::AllocatorType& allocator, Data& data) const
		{
			return static_cast<const Derived*>(this)->To_Json(allocator, data);
		}

		static constexpr const char* key() noexcept 
		{ 
			return Derived::Key();
		}
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_WRITER_H
