#include "Scope.h"

miniT::Scope* miniT::Scope::Find(std::string name)
{
	if (Members.find(name) != Members.end())
		return this;
	if (Parent)
		return Parent->Find(name);
	return nullptr;
}

miniT::TreeNode* miniT::Scope::Get(std::string name)
{
	Scope* scp = Find(name);
	return scp ? scp->Members[name] : nullptr;
}

bool miniT::Scope::Set(std::string name, TreeNode* ast)
{
	Scope* scp = Find(name);
	if (!scp)
		return false;
	scp->Members[name] = ast;
	return true;
}

bool miniT::Scope::Add(std::string name, TreeNode* ast)
{
	bool modified = Find(name);
	Members[name] = ast;
	return modified;
}

miniT::Scope& miniT::Scope::operator=(Scope&& other)
{
	Members = std::move(other.Members);
	Parent = other.Parent;
	Name = std::move(other.Name);
	IsEmpty = false;
	other.IsEmpty = true;
	return *this;
}
