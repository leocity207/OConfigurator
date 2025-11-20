// builder_test.cpp

#include "test_structure.h"
#include "test_structure_builder.h"
#include "test_structure_trait.h"

#include "include/configuration/application/json_builder.h"

#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>

using namespace O::Configuration::Application;

TEST(Builder_From_JSON, numeric_from_string)
{
    constexpr auto json = R"json({ 
        "numeric":
        {
            "tolerance": 1.1
        }
    })json";

    auto expected = Build_From_JSON_String<Numeric>(json);
    ASSERT_TRUE(expected.Has_Value());
    ASSERT_DOUBLE_EQ(expected.Value().Get<Numeric>().tolerance, 1.1);
}

TEST(Builder_From_JSON, various_int_from_string)
{
    constexpr auto json = R"json({
        "various_data": {
            "type": "int",
            "value": 42
        }
    })json";

    auto expected = Build_From_JSON_String<Various_Data>(json);
    ASSERT_TRUE(expected.Has_Value());

    const auto& vd = expected.Value().Get<Various_Data>().type;
    ASSERT_TRUE(std::holds_alternative<Int>(vd));
    ASSERT_EQ(std::get<Int>(vd).value, 42);
}

TEST(Builder_From_JSON, various_double_from_string)
{
    constexpr auto json = R"json({
        "various_data": {
            "type": "double",
            "value": 3.14159
        }
    })json";

    auto expected = Build_From_JSON_String<Various_Data>(json);
    ASSERT_TRUE(expected.Has_Value());

    const auto& vd = expected.Value().Get<Various_Data>().type;
    ASSERT_TRUE(std::holds_alternative<Double>(vd));
    ASSERT_DOUBLE_EQ(std::get<Double>(vd).value, 3.14159);
}

TEST(Builder_From_JSON, various_null_from_string)
{
    constexpr auto json = R"json({
        "various_data": {
            "type": "null"
        }
    })json";

    auto expected = Build_From_JSON_String<Various_Data>(json);
    ASSERT_TRUE(expected.Has_Value());

    const auto& vd = expected.Value().Get<Various_Data>().type;
    ASSERT_TRUE(std::holds_alternative<Null>(vd));
}

TEST(Builder_From_JSON, various_wrong_type_error)
{
    // unsupported type string -> builder should produce module-level error
    constexpr auto json = R"json({
        "various_data": {
            "type": "stringy",
            "value": 1
        }
    })json";

    auto expected = Build_From_JSON_String<Various_Data>(json);
    ASSERT_FALSE(expected.Has_Value());
    // error module name should be "various_data"
    ASSERT_EQ(expected.Error().module_name, std::string_view("various_data"));
    // error id should match Various_Error::WRONG_TYPE
    ASSERT_EQ(expected.Error().error_id, static_cast<int>(Various_Error::WRONG_TYPE));
}

TEST(Builder_From_JSON, parse_error_on_malformed_json)
{
    // malformed JSON -> top-level parse error
    constexpr auto invalid_json = R"json({ "numeric": { "tolerance": 1.23 )json"; // missing closing braces

    auto expected = Build_From_JSON_String<Numeric>(invalid_json);
    ASSERT_FALSE(expected.Has_Value());
    ASSERT_EQ(expected.Error().error_id, static_cast<int>(JSON_PARSING_FAILED));
}

TEST(Builder_From_JSON, numeric_from_file)
{
    // create a temporary file with valid JSON
    const std::string filename = "temp_numeric.json";
    {
        std::ofstream ofs(filename, std::ios::binary);
        ofs << R"json({ "numeric": { "tolerance": 0.25 } })json";
    }

    std::filesystem::path p(filename);
    auto expected = Build_From_JSON_File<Numeric>(p);
    ASSERT_TRUE(expected.Has_Value());
    ASSERT_DOUBLE_EQ(expected.Value().Get<Numeric>().tolerance, 0.25);

    // cleanup
    std::error_code ec;
    std::filesystem::remove(p, ec);
}

TEST(Builder_From_JSON, file_open_error)
{
    // try to open a file that doesn't exist
    std::filesystem::path p("this_file_should_not_exist_12345.json");
    auto expected = Build_From_JSON_File<Numeric>(p);
    ASSERT_FALSE(expected.Has_Value());
    ASSERT_EQ(expected.Error().error_id, static_cast<int>(FILE_OPENING_FAILED));
}
