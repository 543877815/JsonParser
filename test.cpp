#include "pch.h"
#include "json_parser.h"
#include <exception>
#include <memory>
#include <functional>

TEST(json_parser, init)
{
	EXPECT_NO_THROW(json_parser::JsonParser());
}

TEST(json_parser, load_file)
{
	EXPECT_NO_THROW(json_parser::JsonParser("data.json"));
}

void invoke_test(std::function<json_parser::PARSE_STATUS(const std::string&, size_t&)> func, const std::string& str, json_parser::PARSE_STATUS status) {
	size_t idx = 0;
	auto res = func(str, idx);
	EXPECT_EQ(res, status);
};

TEST(json_parser, test_parse)
{
	invoke_test(json_parser::JsonParser::Parse, "null", json_parser::PARSE_OK);
	invoke_test(json_parser::JsonParser::Parse, "true", json_parser::PARSE_OK);
	invoke_test(json_parser::JsonParser::Parse, "false", json_parser::PARSE_OK);
}

TEST(json_parser, test_parse_expect_value)
{
	invoke_test(json_parser::JsonParser::Parse, "", json_parser::PARSE_EXPECT_VALUE);
	invoke_test(json_parser::JsonParser::Parse, " ", json_parser::PARSE_EXPECT_VALUE);
}

TEST(json_parser, test_parse_invalid_value)
{
	invoke_test(json_parser::JsonParser::Parse, "nul", json_parser::PARSE_INVALID_VALUE);
	invoke_test(json_parser::JsonParser::Parse, "?", json_parser::PARSE_INVALID_VALUE);

}

TEST(json_parser, parse_root_not_singular)
{
	invoke_test(json_parser::JsonParser::Parse, "null x", json_parser::PARSE_ROOT_NOT_SINGULAR);
}

TEST(json_parser, json_value)
{
	json_parser::JsonValue json_value = json_parser::JsonValue<json_parser::NULL_VALUE>();
	EXPECT_EQ(json_value.GetType(), json_parser::NULL_VALUE);
}