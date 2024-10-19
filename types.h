#define JSON_PARSER_BEGIN namespace json_parser{
#define JSON_PARSER_END }

JSON_PARSER_BEGIN
typedef enum
{
	UNDEFINED,
	JSON_NULL,
	JSON_FALSE,
	JSON_TRUE,
	JSON_NUMBER,
	JSON_STRING,
	JSON_ARRAY,
	JSON_OBJECT
} VALUE_TYPE;

typedef enum
{
	PARSE_OK = 0,
	PARSE_TO_IMPLEMENTE,
	PARSE_EXPECT_VALUE,
	PARSE_INVALID_VALUE,
	PARSE_ROOT_NOT_SINGULAR
} PARSE_STATUS;

class JsonValueBase {
public:
	virtual ~JsonValueBase() {}
	virtual VALUE_TYPE GetType() const = 0;
protected:
	VALUE_TYPE m_type = VALUE_TYPE::UNDEFINED;
};

template <VALUE_TYPE T>
class JsonValue : public JsonValueBase {
public:
	JsonValue() : m_type(T) {}
	VALUE_TYPE GetType() const override {
		return m_type;
	}
protected:
	VALUE_TYPE m_type;
};

template <>
class JsonValue<JSON_NULL> : public JsonValueBase {
public:
	JsonValue() : m_type(VALUE_TYPE::JSON_NULL) {}
	VALUE_TYPE GetType()  const override {
		return m_type;
	}
private:
	VALUE_TYPE m_type = VALUE_TYPE::JSON_NULL;
};

template <>
class JsonValue<JSON_TRUE> : public JsonValueBase {
public:
	JsonValue() {}
	VALUE_TYPE GetType() const override {
		return m_type;
	}
private:
	VALUE_TYPE m_type = VALUE_TYPE::JSON_TRUE;
};

template <>
class JsonValue<JSON_STRING> : public JsonValueBase {
public:
	JsonValue() = default;
	JsonValue(std::string& data) : m_data(data) {}
	VALUE_TYPE GetType() const override {
		return m_type;
	}
	std::string& GetData() { return m_data; }
private:
	VALUE_TYPE m_type = VALUE_TYPE::JSON_STRING;
	std::string m_data = "";
};

JSON_PARSER_END