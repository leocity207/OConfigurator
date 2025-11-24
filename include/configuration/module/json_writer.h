#ifndef CONFIGURATION_MODULE_JSON_WRITER_H
#define CONFIGURATION_MODULE_JSON_WRITER_H

// STL
#include <string>
#include <utility>
#include <type_traits>

// RAPIDJSON
#include <rapidjson/document.h>

namespace O::Configuration::Module
{

	/**
	 * @brief CRTP base for module JSON writers.
	 *
	 * @tparam Derived The concrete writer implementation.
	 * @tparam Data    The data type this writer serializes.
	 *
	 * @details
	 * The Derived type must implement:
	 * @code
	 * void To_JSON(RapidJSON_Writer& writer, const Data& data) const;
	 * static constexpr const char* Key() noexcept;
	 * @endcode
	 *
	 */
	template<class Derived, class Data>
	struct JSON_Writer
	{
		/**
		 * @brief Forwarding adapter that calls the Derived To_JSON implementation.
		 *
		 *  you don't need to write down the  "Key" inside the writer as this is already done by the Appllication writer. you can directly start by adding Data value
		 * 
		 * @tparam RapidJSON_Writer A RapidJSON writer type (e.g. rapidjson::Writer<...>).
		 * @param writer The RapidJSON writer to write into.
		 * @param data   The module data to serialize.
		 */
		template<class RapidJSON_Writer>
		void To_JSON(RapidJSON_Writer& writer, const Data& data) const
		{
			static_cast<const Derived*>(this)->To_JSON(writer, data);
		}

		/**
		 * @brief Return the JSON key used by this module writer.
		 * 
		 * The value returned by Key will be the Key found inside the json file for this configuration
		 */
		static constexpr const char* Key() noexcept
		{
			return Derived::Key();
		}
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_WRITER_H