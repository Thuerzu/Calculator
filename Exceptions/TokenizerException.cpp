#include "TokenizerException.h"
#include <memory>
#include <stdexcept>

namespace miniT
{
	TokenizerException::TokenizerException(uint32_t line, uint32_t pos, const std::string& message)
		: Line(line), Position(pos), Message(message)
	{
	}
	const char* TokenizerException::what()
	{
		int size = std::snprintf(nullptr, 0, "[TokenizerException] in line {}, {} with error message: {}\n", Line, Position, Message);
		if (size <= 0) throw std::runtime_error("Error while formatting string.");

		auto buffer = std::make_unique<char[]>(size);
		std::snprintf(buffer.get(), size, "[TokenizerException] in line %i, %i with error message: %s\n", Line, Position, Message);

		Output = std::string(buffer.get(), buffer.get() + size - 1);

		return Output.c_str();
	}
}