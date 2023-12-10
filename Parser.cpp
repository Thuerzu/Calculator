#include "Parser.h"
#include "Exceptions/ParserException.h"
#include "Exceptions/TokenizerException.h"
#include <stdexcept>
#include <iostream>

namespace miniT {

	Parser::Parser(Tokenizer* tk, Scope* stdScope) : Tokens(tk), CurrentScope(stdScope)
	{
		if (stdScope)
			StdScope = std::move(*stdScope);
		CurrentScope = &StdScope;
		Parse();
	}

	void Parser::Parse()
	{
		delete ResultTree;
		ResultTree = nullptr;
		try {
			Tokens->ScanNext();
			ResultTree = ParseA();
			if (Tokens->Next.Type != TokenType::endOfFile)
			{
				throw - 1;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << "\n";
		}
		CurrentScope = &StdScope;
	}
	
	TreeNode* Parser::GetTree()
	{
		return ResultTree;
	}

	TreeNode* Parser::ParseA()
	{
		TreeNode* a = ParseE();
		
		if (Tokens->Next.Type == TokenType::assignment)
		{
			switch (a->Type())
			{
			case NodeType::Identifier:
			{
				Tokens->ScanNext();
				AssignmentNode* temp = new AssignmentNode;
				temp->Position = Tokens->Next.Char;
				temp->Line = Tokens->Next.Line;
				temp->Left = a;
				temp->Right = ParseE();
				a = temp;
				break;
			}
			case NodeType::FunctionCall:
			{
				FunctionCall* funcCall = (FunctionCall*)a;
				Identifier* arg = (Identifier*)funcCall->Argument;
				funcCall->Argument = nullptr;
				Function* func = new Function;
				func->Position = a->Position;
				func->Line = a->Line;
				func->Name = funcCall->FunctionName;
				func->Parameter = arg;
				func->Parameter->Parent = &func->ActiveScope;
				funcCall->Argument = nullptr;
				func->Parent = CurrentScope;
				func->ActiveScope.Name = func->Name;
				func->ActiveScope.IsEmpty = true;
				func->ActiveScope.Parent = CurrentScope;
				CurrentScope = &func->ActiveScope;

				Tokens->ScanNext();

				AssignmentNode* fnAssign = new AssignmentNode;
				fnAssign->Position = Tokens->Next.Char;
				fnAssign->Line = Tokens->Next.Line;
				fnAssign->Left = func;
				fnAssign->Right = ParseE();
				delete a;
				a = fnAssign;
				break;
			}
			default:
			{
				break;
			}
			}
		}
		return a;
	}

	TreeNode* Parser::ParseE()
	{
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
				temp->Position = Tokens->Next.Char;
				temp->Line = Tokens->Next.Line;
				a = temp;
				break;
			}
			case TokenType::operatorSub:
			{
				Tokens->ScanNext();
				SubtractNode* temp = new SubtractNode;

				temp->Left = a;
				temp->Right = ParseT();
				temp->Position = Tokens->Next.Char;
				temp->Line = Tokens->Next.Line;
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
				temp->Position = Tokens->Next.Char;
				temp->Line = Tokens->Next.Line;
				a = temp;
				break;
			}
			case TokenType::operatorDiv:
			{
				Tokens->ScanNext();
				DivideNode* temp = new DivideNode;

				temp->Left = a;
				temp->Right = ParseF();
				temp->Position = Tokens->Next.Char;
				temp->Line = Tokens->Next.Line;
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
		switch (Tokens->Next.Type)
		{
		case TokenType::number:
		{
			Number* a = new Number(Tokens->Next.Content);
			a->Position = Tokens->Next.Char;
			a->Line = Tokens->Next.Line;
			Tokens->ScanNext();
			return a;
			break;
		}
		case TokenType::identifier:
		{
			TreeNode* a = new Identifier(Tokens->Next.Content);
			a->Position = Tokens->Next.Char;
			a->Line = Tokens->Next.Line;

			Identifier* id = (Identifier*)a;
			id->Parent = CurrentScope->Find(id->Name);
			if (!id->Parent)
				id->Parent = CurrentScope;
			Tokens->ScanNext();

			switch (Tokens->Next.Type)
			{
			case TokenType::leftP:
			{
				std::string name = id->Name;
				Tokens->ScanNext();
				FunctionCall* temp = new FunctionCall;
				temp->Position = id->Position;
				temp->Line = id->Line;
				temp->FunctionName = name;
				temp->Parent = id->Parent;
				temp->Argument = ParseE();
				if (Tokens->Next.Type != TokenType::rightP)
					throw ParserException(Tokens->Next.Line, Tokens->Next.Char, "Expected ')'");

				Tokens->ScanNext();
				delete a;
				a = temp;
				break;
			}
			default:
			{
				break;
			}
			}

			return a;
			break;
		}
		case TokenType::leftP:
		{
			Tokens->ScanNext();
			TreeNode* a = ParseE();

			if (Tokens->Next.Type == TokenType::rightP)
			{
				Tokens->ScanNext();
				return a; //token scanned successfully 
			}
		
			throw ParserException(Tokens->Next.Line, Tokens->Next.Char, "Expected ')'");

			

			return nullptr; //No closing parenthesis
			break;
		}
		case TokenType::operatorSub:
		{
			Tokens->ScanNext();
			Negate* retVal = new Negate;
			retVal->Position = Tokens->Next.Char;
			retVal->Line = Tokens->Next.Line;
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