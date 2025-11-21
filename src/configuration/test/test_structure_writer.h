#ifndef SRC_CONFIGURATION_TEST_TEST_STRUCTURE_WRITER_H
#define SRC_CONFIGURATION_TEST_TEST_STRUCTURE_WRITER_H

#include <string>
#include <variant>
#include <rapidjson/writer.h>

#include "include/configuration/module/json_writer.h"
#include "test_structure.h"

// =======================================================
//  Numeric_Writer
// =======================================================
struct Numeric_Writer : O::Configuration::Module::JSON_Writer<Numeric_Writer, Numeric>
{
	template<class W>
	void To_JSON(W& w, const Numeric& data) const
	{
		w.StartObject();

		w.Key("tolerance");
		w.Double(data.tolerance);

		w.EndObject();
	}

	static constexpr const char* Key() noexcept { return "numeric"; }
};


// =======================================================
//  Various_Data_Writer
// =======================================================
struct Various_Data_Writer : O::Configuration::Module::JSON_Writer<Various_Data_Writer, Various_Data>
{
	template<class W>
	void To_JSON(W& w, const Various_Data& data) const
	{
		w.StartObject();

		std::visit([&](auto const& v) {
			using T = std::decay_t<decltype(v)>;

			if constexpr (std::is_same_v<T, Int>)
			{
				w.Key("type");
				w.String("int");
				w.Key("value");
				w.Int(v.value);
			}
			else if constexpr (std::is_same_v<T, Double>)
			{
				w.Key("type");
				w.String("double");
				w.Key("value");
				w.Double(v.value);
			}
			else if constexpr (std::is_same_v<T, Null>)
			{
				w.Key("type");
				w.String("null");
				w.Key("value");
				w.Null();
			}
			else
				static_assert(!sizeof(T), "Unhandled type in Various_Data::value");
		}, data.type);

		w.EndObject();
	}

	static constexpr const char* Key() noexcept { return "various_data"; }
};

#endif // SRC_CONFIGURATION_TEST_TEST_STRUCTURE_WRITER_H
