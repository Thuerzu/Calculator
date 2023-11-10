#pragma once
#include "Tokenizer.h"
#include "TreeNodes.h"

namespace miniT {

	class Parser
	{
	public:
		Parser(Tokenizer* tk);
		TreeNode* GetTree();

	private:
		TreeNode* ParseE();
		TreeNode* ParseT();
		TreeNode* ParseF();

	private:
		TreeNode* ResultTree;
		Tokenizer* Tokens;
	};
}

