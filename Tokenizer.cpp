#include "Tokenizer.h"
#include <iostream>

namespace miniT
{
	std::vector<std::tuple<char, TokenType>> Tokenizer::SingleSpecials = {
			{'+', TokenType::operatorAdd},
			{'-', TokenType::operatorSub},
			{'*', TokenType::operatorMult},
			{'/', TokenType::operatorDiv},
			{'(', TokenType::leftP},
			{')', TokenType::rightP}
	};

	bool isDigit(char chr)
	{
		return (chr >= '0') && (chr <= '9');
	}

	Tokenizer::Tokenizer(std::string source)
	{
		State = State::Start;
		Source = source;
	}

	Token* Tokenizer::ScanNext()
	{

		std::cout << "ScanNext() invoked\n";
		static uint32_t i = 0;
		char currentChar = 0;

		State = State::Start;

		Buffer.str("");

		while (true)
		{
			
			if (i >= Source.length())
			{
				if (Buffer.str() != "")
				{
					std::string temp = Buffer.str();
					Buffer.str("");
					Next = Token({ temp, TokenType::number, 0, 0 });
					return &Next;
				}
				Next = Token({ "", TokenType::endOfFile, 0, 0 });
				return &Next;
			}

			currentChar = Source.at(i);

			bool isSpecial = false;
			for (auto[single, type] : SingleSpecials)
			{
				if (single == currentChar) {
					if (Buffer.str() != "")
					{
						Next = Token({ Buffer.str(), TokenType::number, 0, 0 });
						return &Next;
					}

					i++;
					Next = Token({ "", type, 0, 0 });
					return &Next;
				}
			}

			i++;

			if (currentChar == ' ') break;

			if (currentChar == '.') {
				if (State == State::NumFract)
				{
					std::cout << "[ERROR] Too many decimal points in number!\n";
					throw - 1;
				}
				State = State::NumFract;
			}
				
			Buffer << currentChar;

			if (isSpecial) break;
		}

		if (Buffer.str() == "")
		{
			return ScanNext();
		}

		Next = Token({ Buffer.str(), TokenType::number, 0, 0 });
		return &Next;
	}
}