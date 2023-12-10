#pragma once

#include <unordered_map>
#include "../Scope.h"
#include "../TreeNodes.h"

namespace miniT
{
	class Constants
	{
	public:
		static void AddToScope(Scope* scp)
		{
			for (auto [name, val] : BuiltIn)
			{
				scp->Add(name, val);
			}
		}
		static bool IsBuildIn(const std::string& name)
		{
			return BuiltIn.find(name) != BuiltIn.end();
		}
	private:
		static std::unordered_map<std::string, Number*> BuiltIn;
	};
	
}

