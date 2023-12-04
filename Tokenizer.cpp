#include "Tokenizer.h"
#include "Exceptions/TokenizerException.h"
#include <iostream>

namespace miniT
{
	std::unordered_map<char, TokenType> Tokenizer::SingleSpecials = {
		{'+', TokenType::operatorAdd},
		{'-', TokenType::operatorSub},
		{'*', TokenType::operatorMult},
		{'/', TokenType::operatorDiv},
		{'(', TokenType::leftP},
		{')', TokenType::rightP},
		{'=', TokenType::assignment}
	};

	bool isDigit(char chr)
	{
		return (chr >= '0') && (chr <= '9');
	}

	Tokenizer::Tokenizer(const std::string& source)
	{
		SetSource(source);
	}

	void Tokenizer::SetSource(const std::string& source)
	{
		State = State::Start;
		Position = 0;
		Source = source;
		CurrentChar = source.at(0);
	}

	Token* Tokenizer::ScanNext()
	{

		//std::cout << "ScanNext() invoked\n";

		State = State::Start;

		Buffer.str("");

		while (true)
		{
			
			if (Position >= Source.length())
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

			if (isspace(CurrentChar))
			{
				NextChar(CurrentChar);
				continue;
			}

			if (isdigit(CurrentChar))
				return ScanNumber();

			if (isalpha(CurrentChar))
				return ScanID();

			if (SingleSpecials.find(CurrentChar) == SingleSpecials.end())
				throw TokenizerException(0, Position, "Invalid character!");

			Next = { "", SingleSpecials[CurrentChar], 0, Position };
			NextChar(CurrentChar);
			return &Next;
		}
	}
	Token* Tokenizer::ScanNumber()
	{
		uint32_t startPos = Position;
		Buffer.str("");
		while (isdigit(CurrentChar))
		{
			Buffer << CurrentChar;
			if (!NextChar(CurrentChar))
				return &(Next = { Buffer.str(), TokenType::number, 0, startPos });
		}

		if (CurrentChar != '.')
			return &(Next = { Buffer.str(), TokenType::number, 0, startPos });

		Buffer << CurrentChar;
		if (!NextChar(CurrentChar))
			return &(Next = { Buffer.str(), TokenType::number, 0, startPos });

		while (isdigit(CurrentChar))
		{
			Buffer << CurrentChar;
			if (!NextChar(CurrentChar))
				return &(Next = { Buffer.str(), TokenType::number, 0, startPos });
		}

		if (CurrentChar == '.')
			throw TokenizerException(0, Position, "Too many decimal points!");

		return &(Next = { Buffer.str(), TokenType::number, 0, startPos });
	}
	Token* Tokenizer::ScanID()
	{
		uint32_t startPos = Position;
		Buffer.str("");
		while (isalnum(CurrentChar))
		{
			Buffer << CurrentChar;
			if (!NextChar(CurrentChar))
				return &(Next = { Buffer.str(), TokenType::identifier, 0, startPos });
		}
		return &(Next = { Buffer.str(), TokenType::identifier, 0, startPos });
	}
	bool Tokenizer::NextChar(char& c)
	{
		if (++Position >= Source.length())
			return false;
		c = Source.at(Position);
	}
}