#pragma once

#include <unordered_map>
#include <cmath>
#include "../Scope.h"
#include "../TreeNodes.h"

namespace miniT
{
	class BuildInSin : public Function
	{
	public:
		BuildInSin()
		{
			Name = "sin";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::sin(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInCos : public Function
	{
	public:
		BuildInCos()
		{
			Name = "cos";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::cos(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInTan : public Function
	{
	public:
		BuildInTan()
		{
			Name = "tan";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::tan(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInAbs : public Function
	{
	public:
		BuildInAbs()
		{
			Name = "abs";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::abs(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInExp : public Function
	{
	public:
		BuildInExp()
		{
			Name = "exp";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::exp(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInLog : public Function
	{
	public:
		BuildInLog()
		{
			Name = "log";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::log(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class BuildInSqrt : public Function
	{
	public:
		BuildInSqrt()
		{
			Name = "sqrt";
			Parameter = new Identifier("x");
		}
		double virtual Eval() override
		{
			return std::sqrt(((Number*)ActiveScope.Get("x"))->Value);
		}
	};

	class Functions
	{
	public:
		static void AddToScope(Scope* scp)
		{
			for (auto [name, val] : BuildIn)
			{
				val->Parent = scp;
				scp->Add(name, val);
			}
		}
	private:
		static std::unordered_map<std::string, Function*> BuildIn;
	};



	
}

