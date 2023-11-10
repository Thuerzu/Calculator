#pragma once

#include <iostream>
#include <string>


namespace miniT
{
	struct TreeNode
	{
		virtual std::string ToString() = 0;
		virtual double Eval() = 0;
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
	};

	struct Negate : public TreeNode
	{
		TreeNode* Argument;
		virtual std::string ToString() override
		{
			return " (-" + Argument->ToString() + ")";
		}
		virtual double Eval() override {
			return -Argument->Eval();
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
	};
}