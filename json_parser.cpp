#include "pch.h"
#include "json_parser.h"
#ifdef _WIN32
#include <fstream>
#include <sstream>
#include <iostream>
#include <format>
#else
#include "../io/raw_file_utils.h"
#include "../log/log_ohos.h"
#endif // _WIN32
JSON_PARSER_BEGIN
#ifdef _WIN32
JsonParser::JsonParser(const std::string& filename)
{
	m_filename = filename;
	std::ifstream json_file;
	json_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		json_file.open(filename);
		std::stringstream json_stream;
		json_stream << json_file.rdbuf();
		m_document = json_stream.str();
	}
	catch (std::ifstream::failure& e)
	{
		std::cout << std::format("Error reading: {}, FILE_NOT_SUCCESSFULLY_READ: {}", filename, e.what()) << std::endl;
	}
	ParseDocument();
}
#else
JsonParser::JsonParser(const std::string& filename)
{
	auto instance = RawFileUtils::GetInstance();
	instance.ReadBuffer(filename.c_str(), m_raw_data);
	ParseDocument();
}
#endif // _WIN32
JSON_PARSER_END