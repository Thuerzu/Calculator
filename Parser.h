#pragma once
#include "Tokenizer.h"
#include "TreeNodes.h"
#include "Scope.h"
#include <unordered_map>

namespace miniT {

	class Parser
	{
	public:
		Parser(Tokenizer* tk, Scope* stdScope = nullptr);
		void Parse();
		TreeNode* GetTree();

	private:
		TreeNode* ParseE();
		TreeNode* ParseT();
		TreeNode* ParseF();

	private:
		TreeNode* ResultTree;
		Tokenizer* Tokens;
		Scope* CurrentScope;
		Scope StdScope;
	};
}

