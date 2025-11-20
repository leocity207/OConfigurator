#ifndef SRC_CONFIGURATION_TEST_TEST_STRUCTURE_BUILDER_H
#define SRC_CONFIGURATION_TEST_TEST_STRUCTURE_BUILDER_H

#include "include/configuration/module/json_builder.h"

#include "test_structure.h"

enum class Numeric_Error
{
	SHOULD_BE_AND_OBJECT,
	SHOULD_BE_A_DOUBLE,
	NOT_POSITIVE
};

struct Numeric_Builder : public O::Configuration::Module::JSON_Builder<Numeric_Builder, Numeric, Numeric_Error>
{
	std::optional<Numeric_Error> Load_From_Json(const rapidjson::Value& v)
	{
		if (!v.IsObject()) 
			return Numeric_Error::SHOULD_BE_AND_OBJECT;
		if (!(v.HasMember("tolerance") && v["tolerance"].IsNumber()))
			return Numeric_Error::SHOULD_BE_A_DOUBLE;
		if (v["tolerance"].GetDouble() < 0)
			return Numeric_Error::NOT_POSITIVE;
		data.tolerance = v["tolerance"].GetDouble();
		return std::nullopt;
	}

	static constexpr const char* Key() noexcept
	{
		return "numeric";
	}
};

enum class Various_Error
{
	SHOULD_BE_AND_OBJECT,
	SHOULD_BE_A_NUMBER_OR_NULL,
	WRONG_TYPE
};

struct Various_Data_Builder : public O::Configuration::Module::JSON_Builder<Various_Data_Builder, Various_Data, Various_Error>
{
    std::optional<Various_Error> Load_From_Json(const rapidjson::Value& v)
    {
        if (!v.IsObject())
            return Various_Error::SHOULD_BE_AND_OBJECT;

        if (!v.HasMember("type") || !v["type"].IsString())
            return Various_Error::WRONG_TYPE;

        const std::string_view type = v["type"].GetString();

        if (type == "null") {
            data.type = Null{};
            return std::nullopt;
        }
        else if (type == "int") {
            if (!v.HasMember("value") || !(v["value"].IsInt() || v["value"].IsInt64()))
                return Various_Error::SHOULD_BE_A_NUMBER_OR_NULL;
            Int i;
            i.value = v["value"].GetInt();
            data.type = i;
            return std::nullopt;
        }
        else if (type == "double") {
            if (!v.HasMember("value") || !v["value"].IsNumber())
                return Various_Error::SHOULD_BE_A_NUMBER_OR_NULL;
            Double d;
            d.value = v["value"].GetDouble();
            data.type = d;
            return std::nullopt;
        }
        else {
            return Various_Error::WRONG_TYPE;
        }
    }

    static constexpr const char* Key() noexcept
    {
        return "various_data";
    }
};


#endif //SRC_CONFIGURATION_TEST_TEST_STRUCTURE_BUILDER_H