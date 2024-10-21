#define JSON_PARSER_BEGIN namespace json_parser{
#define JSON_PARSER_END }
#include <variant>
#include <string>
JSON_PARSER_BEGIN
typedef enum
{
	UNDEFINED_VALUE,
	NULL_VALUE,
	FALSE_VALUE,
	TRUE_VALUE,
	NUMBER_VALUE,
	STRING_VALUE,
	ARRAY_VALUE,
	OBJECT_VALUE
} JSON_VALUE_TYPE;

typedef enum
{
	PARSE_OK = 0,
	PARSE_TO_IMPLEMENTE,
	PARSE_EXPECT_VALUE,
	PARSE_INVALID_VALUE,
	PARSE_ROOT_NOT_SINGULAR,
	PARSE_NUMBER_TOO_BIG,
	PARSE_MISS_QUOTATION_MARK,
	PARSE_INVALID_STRING_ESCAPE,
	PARSE_INVALID_STRING_CHAR,
	PARSE_INVALID_UNICODE_HEX,
	PARSE_INVALID_UNICODE_SURROGATE
} PARSE_STATUS;

using variant_type = std::variant<std::monostate, bool, double, std::string>;

class JsonValueBase {
public:
	virtual ~JsonValueBase() = default;
	virtual JSON_VALUE_TYPE GetType() const = 0;
	virtual	variant_type GetVariantData() const = 0;
protected:
	JSON_VALUE_TYPE m_type = UNDEFINED_VALUE;
};

template <JSON_VALUE_TYPE T>
class JsonValue : public JsonValueBase {
public:
	JsonValue() = default;
	~JsonValue() = default;
	JSON_VALUE_TYPE GetType() const override { return m_type; }
	variant_type GetVariantData() const override { return std::monostate{}; }
protected:
	JSON_VALUE_TYPE m_type;
};

template <>
class JsonValue<NULL_VALUE> : public JsonValueBase {
public:
	JsonValue() = default;
	~JsonValue() = default;
	JSON_VALUE_TYPE GetType()  const override { return m_type; }
	variant_type GetVariantData() const override { return std::monostate{}; }
private:
	JSON_VALUE_TYPE m_type = NULL_VALUE;
};

template <>
class JsonValue<TRUE_VALUE> : public JsonValueBase {
public:
	JsonValue() = default;
	~JsonValue() = default;
	JSON_VALUE_TYPE GetType() const override { return m_type; }
	variant_type GetVariantData() const override { return true; };
private:
	JSON_VALUE_TYPE m_type = TRUE_VALUE;
};

template <>
class JsonValue<FALSE_VALUE> : public JsonValueBase {
public:
	JsonValue() = default;
	~JsonValue() = default;
	JSON_VALUE_TYPE GetType() const override { return m_type; }
	variant_type GetVariantData() const override { return false; };
private:
	JSON_VALUE_TYPE m_type = FALSE_VALUE;
};

template <>
class JsonValue<NUMBER_VALUE> : public JsonValueBase {
public:
	JsonValue() = default;
	~JsonValue() = default;
	JsonValue(double value) : m_value(value) {}
	JSON_VALUE_TYPE GetType() const override { return m_type; }
	variant_type GetVariantData() const override { return m_value; };
private:
	JSON_VALUE_TYPE m_type = NUMBER_VALUE;
	double m_value = 0.0;
};

template <>
class JsonValue<STRING_VALUE> : public JsonValueBase {
public:
	JsonValue() = default;
	JsonValue(std::string& data) : m_value(data) {}
	JSON_VALUE_TYPE GetType() const override { return m_type; }
	variant_type GetVariantData() const override { return m_value; };
private:
	JSON_VALUE_TYPE m_type = STRING_VALUE;
	std::string m_value = "";
};

JSON_PARSER_END