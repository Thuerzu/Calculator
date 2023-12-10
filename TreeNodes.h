#pragma once

#include <iostream>
#include <string>
#include "Scope.h"
#include "Exceptions/InvalidIdentifierException.h"

namespace miniT
{
	enum class NodeType
	{
		AddSubtract,
		MultDivide,
		Negate,
		Number,
		Identifier,
		Function, FunctionCall,
		Assignment
	};
	struct TreeNode
	{
		virtual std::string ToString() = 0;
		virtual double Eval() = 0;
		virtual NodeType Type() = 0;
		virtual ~TreeNode() {};

		uint32_t Line, Position;
	};

	struct AddNode : public TreeNode
	{
		TreeNode* Left, * Right;
		virtual std::string ToString() override
		{
			return "(" + Left->ToString() + " + " + Right->ToString() + ")";
		}
		virtual double Eval() override
		{
			return Left->Eval() + Right->Eval();
		}
		virtual NodeType Type() override
		{
			return NodeType::AddSubtract;
		}
		~AddNode()
		{
			delete Left, Right;
		}
	};

	struct SubtractNode : public TreeNode
	{
		TreeNode* Left, * Right;
		virtual std::string ToString() override
		{
			return "(" + Left->ToString() + " - " + Right->ToString() + ")";
		}
		virtual double Eval() override
		{
			if (Left && Right)
				return Left->Eval() - Right->Eval();
			//throw 
		}
		virtual NodeType Type() override
		{
			return NodeType::AddSubtract;
		}
		~SubtractNode()
		{
			delete Left, Right;
		}
	};

	struct MultiplyNode : public TreeNode
	{
		TreeNode* Left, * Right;
		virtual std::string ToString() override
		{
			return "(" + Left->ToString() + " * " + Right->ToString() + ")";
		}
		virtual double Eval() override
		{
			return Left->Eval() * Right->Eval();
		}
		virtual NodeType Type() override
		{
			return NodeType::MultDivide;
		}
		~MultiplyNode()
		{
			delete Left, Right;
		}
	};

	struct DivideNode : public TreeNode
	{
		TreeNode* Left, * Right;
		virtual std::string ToString() override
		{
			return "(" + Left->ToString() + " / " + Right->ToString() + ")";
		}
		virtual double Eval() override
		{
			return Left->Eval() / Right->Eval();
		}
		virtual NodeType Type() override
		{
			return NodeType::MultDivide;
		}
		~DivideNode()
		{
			delete Left, Right;
		}
	};

	struct Negate : public TreeNode
	{
		TreeNode* Argument;
		virtual std::string ToString() override
		{
			return " (-" + Argument->ToString() + ")";
		}
		virtual double Eval() override
		{
			return -Argument->Eval();
		}
		virtual NodeType Type() override
		{
			return NodeType::Negate;
		}
		~Negate()
		{
			delete Argument;
		}
	};

	struct Number : public TreeNode
	{
		double Value;

		Number(double val) : Value(val) {}
		Number(std::string content) : Value(stod(content)) {}

		virtual std::string ToString() override
		{
			return std::to_string(Value);
		}
		virtual double Eval() override {
			return Value;
		}
		virtual NodeType Type() override
		{
			return NodeType::Number;
		}
	};

	struct Identifier : public TreeNode
	{
		Scope* Parent;
		std::string Name;

		Identifier(std::string name) : Parent(nullptr), Name(name) {}

		void Assign(double val)
		{
			Parent->Add(Name, new Number(val));
		}

		virtual std::string ToString() override
		{
			return Name;
		}

		virtual double Eval() override
		{
			TreeNode* val;
			val = Parent->Get(Name);
			if (val)
			{
				if (val->Type() == NodeType::Number)
					return ((Number*)val)->Value;
				throw InvalidIdentifierException(Line, Position, "Identifier does not reference a number");
			}
			throw InvalidIdentifierException(Line, Position, "Unknown identifier");
		}

		virtual NodeType Type() override
		{
			return NodeType::Identifier;
		}
	};

	struct Function : public TreeNode
	{
	public:
		std::string Name;
		Identifier* Parameter;
		TreeNode* Expression;
		Scope* Parent;
		Scope ActiveScope;

		virtual NodeType Type() override
		{
			return NodeType::Function;
		}
		virtual double Eval()
		{
			return Expression->Eval();
		}
		virtual std::string ToString() override
		{
			return Name + "(" + Parameter->Name + ")";
		}
		~Function()
		{
			delete Parameter, Expression;
		}
	};

	struct FunctionCall : public TreeNode
	{
		std::string FunctionName;
		TreeNode* Argument;
		Scope* Parent;
		Scope InactiveScope;

		virtual double Eval() override
		{
			Function* fn = (Function*)Parent->Get(FunctionName);
			if (!fn)
				throw InvalidIdentifierException(Line, Position, "Unknown function");
			if (fn->Type() != NodeType::Function)
				throw InvalidIdentifierException(Line, Position, "Identifier does not reference a function");
			InactiveScope = std::move(fn->ActiveScope);
			fn->ActiveScope = Scope();
			fn->ActiveScope.Add(fn->Parameter->Name, new Number(Argument->Eval()));
			fn->ActiveScope.Name = fn->Name;
			double retVal = fn->Eval();
			fn->ActiveScope = std::move(InactiveScope);
			return retVal;
		}
		virtual NodeType Type() override
		{
			return NodeType::FunctionCall;
		}
		virtual std::string ToString() override
		{
			return FunctionName + "(" + Argument->ToString() + ")";
		}
	};

	struct AssignmentNode : public TreeNode
	{
		TreeNode* Left, * Right;

		AssignmentNode() = default;
		AssignmentNode(TreeNode* l, TreeNode* r)
			:	Left(l), Right(r)
		{}

		virtual std::string ToString() override
		{
			return "(" + Left->ToString() + ") = (" + Right->ToString() + ")";
		}

		virtual double Eval() override
		{
			if (Left->Type() == NodeType::Identifier)
			{
				Identifier* id = (Identifier*)Left;
				id->Parent->Add(id->Name, new Number(Right->Eval()));
				return Left->Eval();
			}
			//type == NodeType::Function
			//((Function*)Left)->Expression = Right;
			Function* fn = (Function*)Left;
			fn->Expression = Right;
			fn->Parent->Add(fn->Name, fn);
			Left = nullptr;
			Right = nullptr;
			return -0;
		}

		virtual NodeType Type() override
		{
			return NodeType::Assignment;
		}

		~AssignmentNode()
		{
			delete Left, Right;
		}
	};
}