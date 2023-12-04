#pragma once

#include <iostream>
#include <string>
#include "Scope.h"

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
			return Left->Eval() - Right->Eval();
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
			return ((Number*)Parent->Get(Name))->Value;
		}

		virtual NodeType Type() override
		{
			return NodeType::Identifier;
		}
	};

	struct Function : public TreeNode //Currently unused cuz I'm incapable :,)
	{
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
		Function* Function;
		TreeNode* Argument;
		Scope InactiveScope;

		virtual double Eval() override
		{
			InactiveScope = std::move(Function->ActiveScope);
			Function->ActiveScope = Scope();
			Function->ActiveScope.Add(Function->Parameter->Name, new Number(Argument->Eval()));
			Function->ActiveScope.Name = Function->Name;
			double retVal = Function->Eval();
			Function->ActiveScope = std::move(InactiveScope);
			return retVal;
		}
		virtual NodeType Type() override
		{
			return NodeType::FunctionCall;
		}
		virtual std::string ToString() override
		{
			return Function->Name + "(" + Argument->ToString() + ")";
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