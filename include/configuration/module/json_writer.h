#ifndef CONFIGURATION_MODULE_JSON_WRITER_H
#define CONFIGURATION_MODULE_JSON_WRITER_H

#include <string>
#include <utility>
#include <type_traits>
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
	 * template<class RapidJSON_Writer>
	 * void To_JSON(RapidJSON_Writer& writer, const Data& data) const;
	 * static constexpr const char* Key() noexcept;
	 * @endcode
	 *
	 * The wrapper simply provides a uniform To_JSON call site used by the
	 * application-level JSON writer which iterates all module writers.
	 */
	template<class Derived, class Data>
	struct JSON_Writer
	{
		/**
		 * @brief Forwarding adapter that calls the Derived To_JSON implementation.
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
		 */
		static constexpr const char* Key() noexcept
		{
			return Derived::Key();
		}
	};

} // namespace O::Configuration::Module

#endif // CONFIGURATION_MODULE_JSON_WRITER_H