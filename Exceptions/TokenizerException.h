#pragma once
#include <exception>
#include <string>
namespace miniT 
{
	class TokenizerException : std::exception
	{
	public:
		TokenizerException(uint32_t line, uint32_t pos, const std::string& message);
		const char* what();

	private:
		uint32_t Line, Position;
		std::string Message;
		std::string Output;
	};
}

