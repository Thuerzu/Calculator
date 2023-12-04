#pragma once
#include <memory>
#include <exception>
#include <stdexcept>
#include <string>

namespace miniT
{
	class ParserException
	{
	public:
		ParserException(uint32_t line, uint32_t pos, const std::string& message);
		const char* what();

	private:
		uint32_t Line, Position;
		std::string Message;
		std::string Output;
	};
}

