#pragma once
#include <unordered_map>
#include <memory>
#include <string>

namespace miniT
{	
	struct TreeNode;

	struct Scope
	{
		Scope* Parent;
		std::unordered_map<std::string, TreeNode*> Members;
		std::string Name;
		bool IsEmpty = false;

		/*
		Searches for the name inside the scope and all its parents
		Returns the pointer to the scope that the name was located in
		Returns nullptr if name was not found
		*/

		Scope& operator= (Scope&& other);
		

		Scope* Find(std::string name); 
		bool Add(std::string name, TreeNode* ast);
		TreeNode* Get(std::string name);
		bool Set(std::string name, TreeNode* ast);
	};
}