#include "pch.h"
#include "json_parser.h"
#include <exception>
#include <memory>
TEST(json_parser, init)
{
	EXPECT_NO_THROW(json_parser::JsonParser());
}

TEST(json_parser, load_file)
{
	EXPECT_NO_THROW(json_parser::JsonParser("data.json"));
}

TEST(json_parser, parse_null)
{
	size_t idx = 0;
	auto res = json_parser::JsonParser::ParseNull("null", idx);
	EXPECT_EQ(res, json_parser::PARSE_OK);
}

TEST(json_parser, json_value)
{
	json_parser::JsonValue json_value = json_parser::JsonValue<json_parser::JSON_NULL>();
	EXPECT_EQ(json_value.GetType(), json_parser::JSON_NULL);
	//std::unique_ptr<json_parser::JsonValueBase> a = std::make_unique<json_parser::JsonValue<json_parser::JSON_ARRAY>>();
	std::unique_ptr<json_parser::JsonValueBase> a = std::make_unique<json_parser::JsonValue<json_parser::JSON_NULL>>();
}