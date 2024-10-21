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

void TestNumber(std::function<json_parser::PARSE_STATUS(const std::string&, size_t&, std::shared_ptr<json_parser::JsonValueBase>&)> func,
	const std::string& str, json_parser::PARSE_STATUS status, double expect_number, json_parser::JSON_VALUE_TYPE value_type = json_parser::UNDEFINED_VALUE)
{
	size_t idx = 0;
	std::shared_ptr<json_parser::JsonValueBase> base_value_ptr = std::make_shared<json_parser::JsonValue<json_parser::UNDEFINED_VALUE>>();
	auto res = func(str, idx, base_value_ptr);
	EXPECT_EQ(res, status);
	EXPECT_EQ(base_value_ptr->GetType(), value_type);
	EXPECT_EQ(json_parser::JsonParser::GetValue<double>(base_value_ptr->GetVariantData()), expect_number);
}

void TestString(std::function<json_parser::PARSE_STATUS(const std::string&, size_t&, std::shared_ptr<json_parser::JsonValueBase>&)> func, const std::string& str,
	json_parser::PARSE_STATUS status, const std::string& expect_string, json_parser::JSON_VALUE_TYPE value_type = json_parser::UNDEFINED_VALUE)
{
	size_t idx = 0;
	std::shared_ptr<json_parser::JsonValueBase> base_value_ptr = std::make_shared<json_parser::JsonValue<json_parser::UNDEFINED_VALUE>>();
	auto res = func(str, idx, base_value_ptr);
	EXPECT_EQ(res, status);
	EXPECT_EQ(base_value_ptr->GetType(), value_type);
	EXPECT_EQ(json_parser::JsonParser::GetValue<std::string>(base_value_ptr->GetVariantData()), expect_string);
}

TEST(json_parser, parse_null)
{
	TestError(json_parser::JsonParser::Parse, "null", json_parser::PARSE_OK, json_parser::NULL_VALUE);
}

TEST(json_parser, parse_true)
{
	TestError(json_parser::JsonParser::Parse, "true", json_parser::PARSE_OK, json_parser::TRUE_VALUE);
}

TEST(json_parser, parse_false)
{
	TestError(json_parser::JsonParser::Parse, "false", json_parser::PARSE_OK, json_parser::FALSE_VALUE);
}

TEST(json_parser, parse_number)
{
	TestNumber(json_parser::JsonParser::Parse, "0", json_parser::PARSE_OK, 0.0, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-0", json_parser::PARSE_OK, 0.0, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-0.0", json_parser::PARSE_OK, 0.0, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1", json_parser::PARSE_OK, 1.0, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1", json_parser::PARSE_OK, -1.0, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1.5", json_parser::PARSE_OK, 1.5, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1.5", json_parser::PARSE_OK, -1.5, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "3.1416", json_parser::PARSE_OK, 3.1416, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1E10", json_parser::PARSE_OK, 1E10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1e10", json_parser::PARSE_OK, 1e10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1E+10", json_parser::PARSE_OK, 1E+10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1E-10,", json_parser::PARSE_OK, 1E-10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1E10", json_parser::PARSE_OK, -1E10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1e10", json_parser::PARSE_OK, -1e10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1E+10", json_parser::PARSE_OK, -1E+10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "-1E-10", json_parser::PARSE_OK, -1E-10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1.234E+10", json_parser::PARSE_OK, 1.234E+10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1.234E-10", json_parser::PARSE_OK, 1.234E-10, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1e-10000", json_parser::PARSE_OK, 0.0, json_parser::NUMBER_VALUE); /* must underflow */
	TestNumber(json_parser::JsonParser::Parse, "1.0000000000000002", json_parser::PARSE_OK, 1.0000000000000002, json_parser::NUMBER_VALUE);  /* the smallest number > 1 */
	TestNumber(json_parser::JsonParser::Parse, "4.9406564584124654e-324", json_parser::PARSE_OK, 4.9406564584124654e-324, json_parser::NUMBER_VALUE);  /* minimum denormal */
	TestNumber(json_parser::JsonParser::Parse, "-4.9406564584124654e-324", json_parser::PARSE_OK, -4.9406564584124654e-324, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "2.2250738585072009e-308", json_parser::PARSE_OK, 2.2250738585072009e-308, json_parser::NUMBER_VALUE); /* Max subnormal double */
	TestNumber(json_parser::JsonParser::Parse, "-2.2250738585072009e-308", json_parser::PARSE_OK, -2.2250738585072009e-308, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "2.2250738585072014e-308", json_parser::PARSE_OK, 2.2250738585072014e-308, json_parser::NUMBER_VALUE); /* Min normal positive double */
	TestNumber(json_parser::JsonParser::Parse, "-2.2250738585072014e-308", json_parser::PARSE_OK, -2.2250738585072014e-308, json_parser::NUMBER_VALUE);
	TestNumber(json_parser::JsonParser::Parse, "1.7976931348623157e+308", json_parser::PARSE_OK, 1.7976931348623157e+308, json_parser::NUMBER_VALUE); /* Max double */
	TestNumber(json_parser::JsonParser::Parse, "-1.7976931348623157e+308", json_parser::PARSE_OK, -1.7976931348623157e+308, json_parser::NUMBER_VALUE);
}

TEST(json_parser, parse_string)
{
	TestString(json_parser::JsonParser::Parse, "\"\"", json_parser::PARSE_OK, "", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"Hello\"", json_parser::PARSE_OK, "Hello", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"Hello\\nWorld\"", json_parser::PARSE_OK, "Hello\nWorld", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\\" \\\\ \\/ \\b \\f \\n \\r \\t\"", json_parser::PARSE_OK, "\" \\ / \b \f \n \r \t", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"Hello\\u000AWorld\"", json_parser::PARSE_OK, "Hello\nWorld", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\u0024\"", json_parser::PARSE_OK, "\x24", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\u00A2\"", json_parser::PARSE_OK, "\xC2\xA2", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\u20AC\"", json_parser::PARSE_OK, "\xE2\x82\xAC", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\uD834\\uDD1E\"", json_parser::PARSE_OK, "\xF0\x9D\x84\x9E", json_parser::STRING_VALUE);
	TestString(json_parser::JsonParser::Parse, "\"\\ud834\\udd1e\"", json_parser::PARSE_OK, "\xF0\x9D\x84\x9E", json_parser::STRING_VALUE);
}

TEST(json_parser, parse_expect_value)
{
	TestError(json_parser::JsonParser::Parse, "", json_parser::PARSE_EXPECT_VALUE);
	TestError(json_parser::JsonParser::Parse, " ", json_parser::PARSE_EXPECT_VALUE);
}

TEST(json_parser, parse_invalid_value)
{
	TestError(json_parser::JsonParser::Parse, "nul", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "?", json_parser::PARSE_INVALID_VALUE);

	/* invalid number */
	TestError(json_parser::JsonParser::Parse, "+0", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "+1", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, ".123", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "1.", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "INF", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "inf", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "NAN", json_parser::PARSE_INVALID_VALUE);
	TestError(json_parser::JsonParser::Parse, "nan", json_parser::PARSE_INVALID_VALUE);
}

TEST(json_parser, parse_root_not_singular)
{
	TestError(json_parser::JsonParser::Parse, "null x", json_parser::PARSE_ROOT_NOT_SINGULAR);

	TestError(json_parser::JsonParser::Parse, "0123", json_parser::PARSE_ROOT_NOT_SINGULAR);
	TestError(json_parser::JsonParser::Parse, "0x0", json_parser::PARSE_ROOT_NOT_SINGULAR);
	TestError(json_parser::JsonParser::Parse, "0x123", json_parser::PARSE_ROOT_NOT_SINGULAR);
}

TEST(json_parser, parse_number_too_big)
{
	TestError(json_parser::JsonParser::Parse, "1e309", json_parser::PARSE_NUMBER_TOO_BIG);
	TestError(json_parser::JsonParser::Parse, "-1e309", json_parser::PARSE_NUMBER_TOO_BIG);
}

TEST(json_parser, parse_missing_quotation_mark)
{
	TestError(json_parser::JsonParser::Parse, "\"", json_parser::PARSE_MISS_QUOTATION_MARK);
	TestError(json_parser::JsonParser::Parse, "\"abc", json_parser::PARSE_MISS_QUOTATION_MARK);
}

TEST(json_parser, parse_invalid_string_escape)
{
	TestError(json_parser::JsonParser::Parse, "\"\\v\"", json_parser::PARSE_INVALID_STRING_ESCAPE);
	TestError(json_parser::JsonParser::Parse, "\"\\'\"", json_parser::PARSE_INVALID_STRING_ESCAPE);
	TestError(json_parser::JsonParser::Parse, "\"\\0\"", json_parser::PARSE_INVALID_STRING_ESCAPE);
	TestError(json_parser::JsonParser::Parse, "\"\\x12\"", json_parser::PARSE_INVALID_STRING_ESCAPE);
}

TEST(json_parser, parse_invalid_string_char)
{
	TestError(json_parser::JsonParser::Parse, "\"\x01\"", json_parser::PARSE_INVALID_STRING_CHAR);
	TestError(json_parser::JsonParser::Parse, "\"\x1F\"", json_parser::PARSE_INVALID_STRING_CHAR);
}