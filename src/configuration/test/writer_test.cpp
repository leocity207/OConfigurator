// writer_test.cpp
#include "test_structure.h"
#include "test_structure_builder.h"
#include "test_structure_trait.h"
#include "test_structure_Writer.h"   // writers you added
#include "include/configuration/application/json_writer.h"
#include "include/configuration/application/json_builder.h" // for round-trip tests

#include <gtest/gtest.h>
#include <rapidjson/document.h>
#include <fstream>
#include <filesystem>
#include <string>

using namespace O::Configuration::Application;

static rapidjson::Document Parse_JSON(const std::string& s) {
	rapidjson::Document doc;
	doc.Parse(s.c_str(), s.size());
	return doc;
}

TEST(Writer, Numeric_String)
{
	Container<Numeric> c;
	// set numeric tolerance
	std::get<Numeric>(c.modules).tolerance = 2.5;

	std::string json = Write_As_JSON_String<Numeric>(c);
	rapidjson::Document doc = Parse_JSON(json);
	ASSERT_FALSE(doc.HasParseError());
	ASSERT_TRUE(doc.IsObject());
	ASSERT_TRUE(doc.HasMember("numeric"));
	ASSERT_TRUE(doc["numeric"].IsObject());
	ASSERT_TRUE(doc["numeric"].HasMember("tolerance"));
	ASSERT_TRUE(doc["numeric"]["tolerance"].IsNumber());
	ASSERT_DOUBLE_EQ(doc["numeric"]["tolerance"].GetDouble(), 2.5);
}

TEST(Writer, Various_Int_String)
{
	Container<Various_Data> c;
	Various_Data vd;
	vd.type = Int{ 42 };
	std::get<Various_Data>(c.modules) = vd;

	std::string json = Write_As_JSON_String<Various_Data>(c);
	rapidjson::Document doc = Parse_JSON(json);
	ASSERT_FALSE(doc.HasParseError());
	ASSERT_TRUE(doc.IsObject());
	ASSERT_TRUE(doc.HasMember("various_data"));
	auto& node = doc["various_data"];
	ASSERT_TRUE(node.IsObject());
	ASSERT_TRUE(node.HasMember("type"));
	ASSERT_TRUE(node["type"].IsString());
	ASSERT_STREQ(node["type"].GetString(), "int");
	ASSERT_TRUE(node.HasMember("value"));
	ASSERT_TRUE(node["value"].IsInt());
	ASSERT_EQ(node["value"].GetInt(), 42);
}

TEST(Writer, Various_Double_String)
{
	Container<Various_Data> c;
	Various_Data vd;
	vd.type = Double{ 3.14159 };
	std::get<Various_Data>(c.modules) = vd;

	std::string json = Write_As_JSON_String<Various_Data>(c);
	rapidjson::Document doc = Parse_JSON(json);
	ASSERT_FALSE(doc.HasParseError());
	ASSERT_TRUE(doc.IsObject());
	ASSERT_TRUE(doc.HasMember("various_data"));
	auto& node = doc["various_data"];
	ASSERT_TRUE(node.IsObject());
	ASSERT_TRUE(node.HasMember("type"));
	ASSERT_STREQ(node["type"].GetString(), "double");
	ASSERT_TRUE(node.HasMember("value"));
	ASSERT_TRUE(node["value"].IsNumber());
	ASSERT_DOUBLE_EQ(node["value"].GetDouble(), 3.14159);
}

TEST(Writer, Various_Null_String)
{
	Container<Various_Data> c;
	Various_Data vd;
	vd.type = Null{};
	std::get<Various_Data>(c.modules) = vd;

	std::string json = Write_As_JSON_String<Various_Data>(c);
	rapidjson::Document doc = Parse_JSON(json);
	ASSERT_FALSE(doc.HasParseError());
	ASSERT_TRUE(doc.IsObject());
	ASSERT_TRUE(doc.HasMember("various_data"));
	auto& node = doc["various_data"];
	ASSERT_TRUE(node.IsObject());
	ASSERT_TRUE(node.HasMember("type"));
	ASSERT_STREQ(node["type"].GetString(), "null");
	ASSERT_TRUE(node.HasMember("value"));
	ASSERT_TRUE(node["value"].IsNull());
}

TEST(Writer, Numeric_File)
{
	Container<Numeric> c;
	std::get<Numeric>(c.modules).tolerance = 0.25;

	const std::filesystem::path tmpfile = "tmp_numeric_writer_test.json";
	// remove prior if exists
	std::error_code ec;
	std::filesystem::remove(tmpfile, ec);

	auto err = Write_As_JSON_File<Numeric>(c, tmpfile);
	ASSERT_FALSE(err.has_value()) << "Write_as_JSON_File returned error";

	// read file
	std::ifstream ifs(tmpfile, std::ios::binary);
	ASSERT_TRUE(ifs.good());
	std::string contents{std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
	rapidjson::Document doc;
	doc.Parse(contents.c_str(), contents.size());
	ASSERT_FALSE(doc.HasParseError());
	ASSERT_TRUE(doc.HasMember("numeric"));
	ASSERT_DOUBLE_EQ(doc["numeric"]["tolerance"].GetDouble(), 0.25);

	// cleanup
	std::filesystem::remove(tmpfile, ec);
}

TEST(Writer, File_Open_Failure)
{
	Container<Numeric> c;
	std::get<Numeric>(c.modules).tolerance = 1.0;

	// attempt to write to a file in a non-existent directory
	const std::filesystem::path badpath = "nonexistent_dir/subdir/output.json";
	// ensure directory doesn't exist
	std::error_code ec;
	std::filesystem::remove_all("nonexistent_dir", ec);

	auto err = Write_As_JSON_File<Numeric>(c, badpath);
	ASSERT_TRUE(err.has_value());
	// we don't assert exact enum value because your Write_Error may be named differently;
	// but if your enum provides FILE_OPEN_FAILED, you can do:
	// ASSERT_EQ(*err, Write_Error::FILE_OPEN_FAILED);
}

TEST(Writer, Roundtrip_String_ReadBack_Numeric)
{
	Container<Numeric> c;
	std::get<Numeric>(c.modules).tolerance = 7.125;

	std::string json = Write_As_JSON_String<Numeric>(c);

	// parse back with builder
	auto read = Build_From_JSON_String<Numeric>(json);
	ASSERT_TRUE(read.Has_Value());
	auto &read_numeric = read.Value().Get<Numeric>();
	ASSERT_DOUBLE_EQ(read_numeric.tolerance, 7.125);
}

TEST(Writer, Roundtrip_String_ReadBack_Various)
{
	Container<Various_Data> c;
	Various_Data vd;
	vd.type = Double{ 9.81 };
	std::get<Various_Data>(c.modules) = vd;

	std::string json = Write_As_JSON_String<Various_Data>(c);

	auto read = Build_From_JSON_String<Various_Data>(json);
	ASSERT_TRUE(read.Has_Value());
	auto &rd = read.Value().Get<Various_Data>();
	ASSERT_TRUE(std::holds_alternative<Double>(rd.type));
	ASSERT_DOUBLE_EQ(std::get<Double>(rd.type).value, 9.81);
}
