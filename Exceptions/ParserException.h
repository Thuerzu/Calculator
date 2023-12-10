#pragma once
#include <memory>
#include <stdexcept>
#include <memory>
#include <string>

namespace miniT
{
	class ParserException : virtual public std::exception
	{
	public:
		ParserException(uint32_t line, uint32_t pos, const std::string& message)
			: Line(line), Position(pos), Message(message)
		{
			const char* tmpl = "[ParserException] in line %i, %i with error message: %s\n";
			int size = std::snprintf(nullptr, 0, "[ParserException] in line %i, %i with error message: %s\n", Line, Position, Message.c_str());
			if (size <= 0) throw std::runtime_error("Error while formatting string.");

			auto buffer = std::make_unique<char[]>(size);
			std::snprintf(buffer.get(), size, "[ParserException] in line %i, %i with error message: %s\n", Line, Position, Message.c_str());

			Output = std::string(buffer.get(), buffer.get() + size - 1);
		}
		virtual const char* what() const override
		{
			return Output.c_str();
		}

	private:
		uint32_t Line, Position;
		std::string Message;
		std::string Output;
	};
}

