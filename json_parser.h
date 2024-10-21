
#include <string>
#include "./types.h"


JSON_PARSER_BEGIN

template<typename T, typename = void>
struct is_allowed_type : std::false_type {};

template<typename T>
struct is_allowed_type<T, std::void_t<>> : std::bool_constant<
	std::is_same<T, int>::value ||
	std::is_same<T, double>::value ||
	std::is_same<T, std::string>::value
> {};

class JsonParser {
public:
	JsonParser() {}
	JsonParser(const std::string& filename);
	~JsonParser() = default;

	static PARSE_STATUS Parse(const std::string& str, size_t& start_idx, std::shared_ptr<JsonValueBase>& ret_ptr)
	{
		PARSE_STATUS res;
		JsonParser::ParseWhiteSpace(str, start_idx);
		std::shared_ptr<JsonValueBase> base_value_ptr = nullptr;
		if ((res = JsonParser::ParseValue(str, start_idx, base_value_ptr)) == PARSE_OK) {
			JsonParser::ParseWhiteSpace(str, start_idx);
			if (str.size() != start_idx && str[start_idx] != ',' && str[start_idx] != '\n')
				return PARSE_ROOT_NOT_SINGULAR;
		}
		if (base_value_ptr)
			ret_ptr = base_value_ptr;
		return res;
	}

	static PARSE_STATUS ParseValue(const std::string& str, size_t& idx, std::shared_ptr<JsonValueBase>& base_value_ptr)
	{
		if (idx == str.size())
			return PARSE_EXPECT_VALUE;
		switch (str[0])
		{
		case 't': return ParseLiteral(str, idx, "true", TRUE_VALUE, base_value_ptr);
		case 'f': return ParseLiteral(str, idx, "false", FALSE_VALUE, base_value_ptr);
		case 'n': return ParseLiteral(str, idx, "null", NULL_VALUE, base_value_ptr);
		case '"': return ParseString(str, idx, base_value_ptr);
		default:  return ParseNumber(str, idx, base_value_ptr);

		}
	}

	static PARSE_STATUS ParseNumber(const std::string& str, size_t& start_idx, std::shared_ptr<JsonValueBase>& base_value_ptr)
	{
		size_t i = start_idx;
		// parse negative
		if (str[i] == '-') i++;

		// parse integer;
		if (str[i] == '0') i++;
		else {
			if (!isDigit1to9(str[i])) return PARSE_INVALID_VALUE;
			for (i++; isDigit(str[i]); i++);
		}
		// parse decimal
		if (str[i] == '.')
		{
			i++;
			if (!isDigit(str[i])) return PARSE_INVALID_VALUE;
			for (i++; isDigit(str[i]); i++);
		}
		// parse exponent
		if (str[i] == 'e' || str[i] == 'E')
		{
			i++;
			if (str[i] == '+' || str[i] == '-') i++;
			if (!isDigit(str[i])) return PARSE_INVALID_VALUE;
			for (i++; isDigit(str[i]); i++);
		}
		char* end_ptr = nullptr;
		std::string_view sv(str);
		sv = sv.substr(start_idx, i - start_idx);
		errno = 0;
		double value = strtod(sv.data(), &end_ptr);
		if (errno == ERANGE && (value == HUGE_VAL || value == -HUGE_VAL))
			return PARSE_NUMBER_TOO_BIG;

		if (end_ptr - sv.data() != i - start_idx) {
			return PARSE_ROOT_NOT_SINGULAR;
		}
		base_value_ptr = std::make_shared<JsonValue<NUMBER_VALUE>>(value);
		start_idx = i;
		return PARSE_OK;
	}

	static PARSE_STATUS ParseString(const std::string& str, size_t& start_idx, std::shared_ptr<JsonValueBase>& base_value_ptr)
	{
		assert(str[start_idx++] == '\"');
		size_t i = start_idx;
		char ch = '0';
		std::string ret = "";
		while (true)
		{
			if (i == str.size()) // case '\0'
			{
				return PARSE_MISS_QUOTATION_MARK;
			}

			ch = str[i];
			if (ch == '\"') {
				base_value_ptr = std::make_shared<JsonValue<STRING_VALUE>>(ret);
				start_idx = i + 1;
				return PARSE_OK;
			}
			else if (ch == '\\') {
				i++;
				switch (str[i])
				{
				case '\"': ret = ret + '"'; break;
				case '\\': ret = ret + '\\'; break;
				case '/': ret = ret + '/'; break;
				case 'b': ret = ret + '\b';  break;
				case 'f': ret = ret + '\f';  break;
				case 'n': ret = ret + '\n'; break;
				case 'r': ret = ret + '\r'; break;
				case 't': ret = ret + '\t'; break;
				case 'u':
					unsigned int u, u2;
					if (!ParseHex4(str, i, u)) {
						return PARSE_INVALID_UNICODE_HEX;
					}
					if (u >= 0xD800 && u <= 0xDBFF) { /* surrogate pair */
						if (str[++i] != '\\')
							return PARSE_INVALID_UNICODE_SURROGATE;
						if (str[++i] != 'u')
							return PARSE_INVALID_UNICODE_SURROGATE;
						if (!ParseHex4(str, i, u2))
							return PARSE_INVALID_UNICODE_HEX;
						if (u2 < 0xDC00 || u2 > 0xDFFF)
							return PARSE_INVALID_UNICODE_SURROGATE;
						u = (((u - 0xD800) << 10) | (u2 - 0xDC00)) + 0x10000;
					}
					ret = ret + EncodeUtf8(u);
					break;
				default:
					return PARSE_INVALID_STRING_ESCAPE;
				}
			}
			else {
				if (static_cast<unsigned char>(ch) < 0x20)
					return PARSE_INVALID_STRING_CHAR;
				ret = ret + ch;
				//std::cout << ret << std::endl;
			}
			i++;
		}
	}

	static PARSE_STATUS ParseLiteral(const std::string& str, size_t& start_idx, const std::string& literal, JSON_VALUE_TYPE value_type, std::shared_ptr<JsonValueBase>& base_value_ptr)
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

	static bool ParseHex4(const std::string& str, size_t& start_idx, unsigned& u)
	{
		u = 0;
		for (int i = 0; i < 4; i++)
		{
			start_idx++;
			char ch = str[start_idx];
			u <<= 4;
			if (ch >= '0' && ch <= '9') u |= ch - '0';
			else if (ch >= 'A' && ch <= 'F') u |= ch - ('A' - 10);
			else if (ch >= 'a' && ch <= 'f') u |= ch - ('a' - 10);
			else return false;
		}
		return true;
	}

	static std::string EncodeUtf8(unsigned u)
	{
		std::string ret = "";
		if (u <= 0x7F)
		{
			ret.push_back(static_cast<char>(u & 0xFF));
		}
		else if (u <= 0x7FF) {
			ret.push_back(static_cast<char>(0xC0 | ((u >> 6) & 0xFF)));
			ret.push_back(static_cast<char>(0x80 | (u & 0x3F)));
		}
		else if (u <= 0xFFFF) {
			ret.push_back(static_cast<char>(0xE0 | ((u >> 12) & 0xFF)));
			ret.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3F)));
			ret.push_back(static_cast<char>(0x80 | (u & 0x3F)));
		}
		else {
			assert(u <= 0x10FFFF);
			ret.push_back(static_cast<char>(0xF0 | ((u >> 18) & 0xFF)));
			ret.push_back(static_cast<char>(0x80 | ((u >> 12) & 0x3F)));
			ret.push_back(static_cast<char>(0x80 | ((u >> 6) & 0x3F)));
			ret.push_back(static_cast<char>(0x80 | (u & 0x3F)));
		}
		return ret;
	}

	template<typename T>
	static auto GetValue(const variant_type& vt) -> std::enable_if_t<is_allowed_type<T>::value, T>
	{
		if (std::holds_alternative<std::monostate>(vt))
		{
			throw std::runtime_error("Trying to get value of type T but variant contains std::monostate type. Expected types are [bool, double, std::string]");
		}
		if (!std::holds_alternative<T>(vt))
		{
			throw std::runtime_error("Trying to get value of type T but variant contains different type.");
		}
		return std::get<T>(vt);
	}

	static bool isDigit1to9(char c)
	{
		return c >= '1' && c <= '9';
	}

	static bool isDigit(char c)
	{
		return c >= '0' && c <= '9';
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