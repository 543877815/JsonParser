
#include <string>
#include "./types.h"


JSON_PARSER_BEGIN
class JsonParser {
public:
	JsonParser() {}
	JsonParser(const std::string& filename);
	~JsonParser() = default;

	static PARSE_STATUS ParseNull(const std::string& str, size_t& idx) {
		std::shared_ptr<JsonValueBase> base_value_ptr = nullptr;
		JsonParser::Parse(str, idx, base_value_ptr, "null");
		return PARSE_OK;
	}

	static PARSE_STATUS ParseTrue()
	{
		return PARSE_OK;
	}

	static PARSE_STATUS ParseFalse()
	{
		return PARSE_OK;
	}

	static PARSE_STATUS Parse(const std::string& str, size_t& idx, std::shared_ptr<JsonValueBase> base_value_ptr, const std::string& value)
	{
		if (idx + value.size() >= str.size())
			return PARSE_EXPECT_VALUE;

		PARSE_STATUS res;
		JsonParser::ParseWhiteSpace(str, idx);
		if ((res = JsonParser::ParseValue(str, idx, base_value_ptr, value))) {
			JsonParser::ParseWhiteSpace(str, idx);
			if (str.size() != idx && str[idx] != ',' && str[idx] != '\n')
				res = PARSE_ROOT_NOT_SINGULAR;
		}
		return res;
	}

	static PARSE_STATUS ParseValue(const std::string& str, size_t& idx, std::shared_ptr<JsonValueBase> base_value_ptr, const std::string& value)
	{
		if (idx + value.size() >= str.size())
			return PARSE_EXPECT_VALUE;
		switch (str[idx])
		{
		case 't': return ParseLiteral(str, idx, base_value_ptr, "true", VALUE_TYPE::JSON_TRUE);
		case 'f': return ParseLiteral(str, idx, base_value_ptr, "false", VALUE_TYPE::JSON_FALSE);
		case 'n': return ParseLiteral(str, idx, base_value_ptr, "null", VALUE_TYPE::JSON_NULL);
		default:  return PARSE_TO_IMPLEMENTE; // TODO
		case '\0': return PARSE_EXPECT_VALUE;
		}
	}

	static PARSE_STATUS ParseLiteral(const std::string& str, size_t& idx, std::shared_ptr<JsonValueBase> base_value_ptr, const std::string& literal, VALUE_TYPE value_type)
	{
		size_t size = literal.size();
		for (size_t i = 0; i < size; i++)
		{
			if (str[idx + i] != literal[i])
			{
				return PARSE_INVALID_VALUE;
			}
		}
		idx += size;
		base_value_ptr = std::make_shared<JsonValue<JSON_NULL>>();
		return PARSE_OK;
	}

	static void ParseWhiteSpace(const std::string& str, size_t& idx)
	{
		size_t size = str.size();
		while (idx < size && (str[idx] == ' ' || str[idx] == '\t' || str[idx] == '\n' || str[idx] == '\r'))
			idx++;
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