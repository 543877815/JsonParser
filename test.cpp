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

void TestError(std::function<json_parser::PARSE_STATUS(const std::string&, size_t&, std::shared_ptr<json_parser::JsonValueBase>&)> func, const std::string& str, json_parser::PARSE_STATUS status, json_parser::JSON_VALUE_TYPE value_type = json_parser::UNDEFINED_VALUE) {
	size_t idx = 0;
	std::shared_ptr<json_parser::JsonValueBase> base_value_ptr = std::make_shared<json_parser::JsonValue<json_parser::UNDEFINED_VALUE>>();
	auto res = func(str, idx, base_value_ptr);
	EXPECT_EQ(res, status);
	EXPECT_EQ(base_value_ptr->GetType(), value_type);
};

void TestNumber(std::function<json_parser::PARSE_STATUS(const std::string&, size_t&, std::shared_ptr<json_parser::JsonValueBase>&)> func, const std::string& str, json_parser::PARSE_STATUS status, double number, json_parser::JSON_VALUE_TYPE value_type = json_parser::UNDEFINED_VALUE)
{
	size_t idx = 0;
	std::shared_ptr<json_parser::JsonValueBase> base_value_ptr = std::make_shared<json_parser::JsonValue<json_parser::NUMBER_VALUE>>();
	auto res = func(str, idx, base_value_ptr);
	EXPECT_EQ(res, status);
	EXPECT_EQ(base_value_ptr->GetType(), value_type);
	//EXPECT_EQ(base_value_ptr->GetVariantData(), value_type);
}

TEST(json_parser, test_parse)
{
	TestError(json_parser::JsonParser::Parse, "null", json_parser::PARSE_OK, json_parser::NULL_VALUE);
	TestError(json_parser::JsonParser::Parse, "true", json_parser::PARSE_OK, json_parser::TRUE_VALUE);
	TestError(json_parser::JsonParser::Parse, "false", json_parser::PARSE_OK, json_parser::FALSE_VALUE);
}

TEST(json_parser, parse_number)
{

}

TEST(json_parser, test_parse_expect_value)
{
	TestError(json_parser::JsonParser::Parse, "", json_parser::PARSE_EXPECT_VALUE);
	TestError(json_parser::JsonParser::Parse, " ", json_parser::PARSE_EXPECT_VALUE);
}

TEST(json_parser, test_parse_invalid_value)
{
	TestError(json_parser::JsonParser::Parse, "nul", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "?", json_parser::PARSE_INVALID_VALUE);

}

TEST(json_parser, parse_root_not_singular)
{
	TestError(json_parser::JsonParser::Parse, "null x", json_parser::PARSE_ROOT_NOT_SINGULAR);
}
