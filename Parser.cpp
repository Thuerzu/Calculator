#include "Parser.h"
#include <iostream>

namespace miniT {

	Parser::Parser(Tokenizer* tk) : Tokens(tk)
	{
		Tokens->ScanNext();
		ResultTree = ParseE();
		if (Tokens->Next.Type != TokenType::endOfFile)
		{
			throw - 1;
		}
	}
	
	TreeNode* Parser::GetTree()
	{
		return ResultTree;
	}

	TreeNode* Parser::ParseE()
	{
		std::cout << "ParseE() invoked\n";
		TreeNode* a = ParseT();

		while (true)
		{
			switch (Tokens->Next.Type)
			{
			case TokenType::operatorAdd:
			{
				Tokens->ScanNext();
				AddNode* temp = new AddNode;

				temp->Left = a;
				temp->Right = ParseT();

				a = temp;
				break;
			}
			case TokenType::operatorSub:
			{
				Tokens->ScanNext();
				SubtractNode* temp = new SubtractNode;

				temp->Left = a;
				temp->Right = ParseT();

				a = temp;
				break;
			}
			default:
			{
				return a;
				break;
			}
			}
		}


		return nullptr;
	}

	TreeNode* Parser::ParseT()
	{
		std::cout << "ParseT() invoked\n";

		TreeNode* a = ParseF();

		while (true)
		{
			switch (Tokens->Next.Type)
			{
			case TokenType::operatorMult:
			{
				Tokens->ScanNext();
				MultiplyNode* temp = new MultiplyNode;

				temp->Left = a;
				temp->Right = ParseF();

				a = temp;
				break;
			}
			case TokenType::operatorDiv:
			{
				Tokens->ScanNext();
				DivideNode* temp = new DivideNode;

				temp->Left = a;
				temp->Right = ParseF();

				a = temp;
				break;
			}
			default:
			{
				return a;
				break;
			}
			}
		}


		return nullptr;
	}

	TreeNode* Parser::ParseF()
	{
		std::cout << "ParseF() invoked\n";

		std::cout << Tokens->Next.ToString() << "\n";

		switch (Tokens->Next.Type)
		{
		case TokenType::number:
		{
			Number* a = new Number(Tokens->Next.Content);
			Tokens->ScanNext();
			return a;
			break;
		}
		case TokenType::leftP:
		{
			Tokens->ScanNext();
			TreeNode* a = ParseE();

			if (a == nullptr) return nullptr; //something went wrong

			if (Tokens->Next.Type == TokenType::rightP)
			{
				Tokens->ScanNext();
				return a; //token scanned successfully
			}

			return nullptr; //No closing parenthesis
			break;
		}
		case TokenType::operatorSub:
		{
			Tokens->ScanNext();
			Negate* retVal = new Negate;
			retVal->Argument = ParseF();
			return retVal;
			break;
		}
		default:
			return nullptr;
			break;
		};
	}
}