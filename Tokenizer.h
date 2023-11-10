#pragma once

#include <string>
#include <tuple>
#include <vector>
#include <sstream>

namespace miniT
{
	bool isDigit(char chr);

	double stringToDouble(std::string& str);

	
	enum class TokenType
	{
		operatorAdd,
		operatorSub,
		operatorMult,
		operatorDiv,
		leftP, rightP,
		number, endOfFile
	};

	struct Token
	{
		std::string Content;
		TokenType Type;
		uint32_t Line, Char;

		std::string ToString() {
			switch (Type)
			{
			case miniT::TokenType::operatorAdd:
				return "add";
				break;
			case miniT::TokenType::operatorSub:
				return "sub";
				break;
			case miniT::TokenType::operatorMult:
				return "mult";
				break;
			case miniT::TokenType::operatorDiv:
				return "div";
				break;
			case miniT::TokenType::leftP:
				return "leftP";
				break;
			case miniT::TokenType::rightP:
				return "rightP";
				break;
			case miniT::TokenType::number:
				return "Number: " + Content;
				break;
			case miniT::TokenType::endOfFile:
				return "EOF";
				break;
			default:
				return std::string();
				break;
			}
		}
	};
	class Tokenizer
	{
	public:
		Tokenizer(std::string source);
		Token* ScanNext();
	public:
		enum class State
		{
			Start,			//at the start of a new token
			NumInt,			//inside the integer part of a number
			NumFract,		//inside the fractal part of a number
			String,			//inside a string
			Text			//inside an ID
		} State;
		static std::vector<std::tuple<char, TokenType>> SingleSpecials;

		Token Next;
		std::string Source;
		std::stringstream Buffer;

	};
}