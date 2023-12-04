#include "Parser.h"
#include "Exceptions/ParserException.h"
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
		Tokens->ScanNext();
		ResultTree = ParseE();
		CurrentScope = &StdScope;
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
		//std::cout << "ParseE() invoked\n";
		TreeNode* a = ParseT();
		
		if (a->Type() == NodeType::Identifier)
		{
			switch (Tokens->Next.Type)
			{
			case TokenType::leftP:
			{
				std::string name = ((Identifier*)a)->Name;
				Tokens->ScanNext();
				FunctionCall* temp = new FunctionCall;
				temp->Argument = ParseE();
				if (Tokens->Next.Type != TokenType::rightP)
					throw ParserException(Tokens->Next.Line, Tokens->Next.Char, "Expected ')'");

				Tokens->ScanNext();

				//if (temp->Argument->Type() == NodeType::Identifier)
				if (Tokens->Next.Type == TokenType::assignment)
				{
					Identifier* arg = (Identifier*)temp->Argument;
					Function* func = new Function;
					func->Name = name;
					func->Parameter = arg;
					func->Parameter->Parent = &func->ActiveScope;
					temp->Argument = nullptr;
					func->Parent = CurrentScope;
					func->ActiveScope.Name = func->Name;
					func->ActiveScope.IsEmpty = true;
					func->ActiveScope.Parent = CurrentScope;
					CurrentScope = &func->ActiveScope;

					if (Tokens->Next.Type != TokenType::assignment)
						throw ParserException(Tokens->Next.Line, Tokens->Next.Char, "Expected '='");

					Tokens->ScanNext();

					AssignmentNode* fnAssign = new AssignmentNode;
					fnAssign->Left = func;
					fnAssign->Right = ParseE();

					delete a;
					a = fnAssign;

					break;
				}
				//function call
				temp->Function = (Function*)((Identifier*)a)->Parent->Get(name);
				delete a;
				a = temp;
				break;
			}
			case TokenType::assignment:
			{
				Tokens->ScanNext();
				AssignmentNode* temp = new AssignmentNode;
				temp->Left = a;
				temp->Right = ParseE();
				a = temp;
				break;
			}
			default:
			{
				break;
			}
			}
		}

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
		//std::cout << "ParseT() invoked\n";

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
		//std::cout << "ParseF() invoked\n";

		//std::cout << Tokens->Next.ToString() << "\n";

		switch (Tokens->Next.Type)
		{
		case TokenType::number:
		{
			Number* a = new Number(Tokens->Next.Content);
			Tokens->ScanNext();
			return a;
			break;
		}
		case TokenType::identifier:
		{
			Identifier* a = new Identifier(Tokens->Next.Content);
			a->Parent = CurrentScope->Find(a->Name);
			if (!a->Parent)
				a->Parent = CurrentScope;
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