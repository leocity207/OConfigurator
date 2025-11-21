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
		template<class RapidJSON_Writer>
		void To_JSON(RapidJSON_Writer& writer, const Data& data) const
		{
			static_cast<const Derived*>(this)->To_JSON(writer, data);
		}

		static constexpr const char* Key() noexcept 
		{ 
			return Derived::Key();
		}
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_WRITER_H
