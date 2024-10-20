
#include <string>
#include "./types.h"


JSON_PARSER_BEGIN
class JsonParser {
public:
	JsonParser() {}
	JsonParser(const std::string& filename);
	~JsonParser() = default;

	static PARSE_STATUS Parse(const std::string& str, size_t& start_idx)
	{
		PARSE_STATUS res;
		JsonParser::ParseWhiteSpace(str, start_idx);
		std::shared_ptr<JsonValueBase> base_value_ptr = nullptr;
		if ((res = JsonParser::ParseValue(str, start_idx, base_value_ptr)) == PARSE_OK) {
			JsonParser::ParseWhiteSpace(str, start_idx);
			if (str.size() != start_idx && str[start_idx] != ',' && str[start_idx] != '\n')
				res = PARSE_ROOT_NOT_SINGULAR;
		}
		return res;
	}

	static PARSE_STATUS ParseValue(const std::string& str, size_t& idx, std::shared_ptr<JsonValueBase> base_value_ptr)
	{
		if (idx == str.size())
			return PARSE_EXPECT_VALUE;
		switch (str[0])
		{
		case 't': return ParseLiteral(str, idx, "true", TRUE_VALUE, base_value_ptr);
		case 'f': return ParseLiteral(str, idx, "false", FALSE_VALUE, base_value_ptr);
		case 'n': return ParseLiteral(str, idx, "null", NULL_VALUE, base_value_ptr);
		default:  return PARSE_INVALID_VALUE; // TODO
		}
	}

	static PARSE_STATUS ParseLiteral(const std::string& str, size_t& start_idx, const std::string& literal, JSON_VALUE_TYPE value_type, std::shared_ptr<JsonValueBase> base_value_ptr)
	{
		if (start_idx + literal.size() > str.size())
			return PARSE_INVALID_VALUE;
		size_t size = literal.size();
		for (size_t i = 0; i < size; i++)
		{
			if (str[start_idx + i] != literal[i])
			{
				return PARSE_INVALID_VALUE;
			}
		}
		start_idx += size;

		switch (value_type)
		{
		case NULL_VALUE: {
			base_value_ptr = std::make_shared<JsonValue<NULL_VALUE>>();
			break;
		}
		case TRUE_VALUE: {
			base_value_ptr = std::make_shared<JsonValue<TRUE_VALUE>>();
			break;
		}
		case FALSE_VALUE: {
			base_value_ptr = std::make_shared<JsonValue<FALSE_VALUE>>();
			break;
		}
		default:
			break;
		}
		return PARSE_OK;
	}

	static void ParseWhiteSpace(const std::string& str, size_t& start_idx)
	{
		size_t size = str.size();
		while (start_idx < size && (str[start_idx] == ' ' || str[start_idx] == '\t' || str[start_idx] == '\n' || str[start_idx] == '\r'))
			start_idx++;
	}

private:
	std::string m_filename;
	std::string m_document;
	size_t m_ptr = 0;

	void ParseDocument() {
		// TODO
		size_t size = m_document.size();
		//while (m_ptr < size) {
			//ParseNull();
			//ParseTrue();
			//ParseFalse();
		//}
	}
};
JSON_PARSER_END