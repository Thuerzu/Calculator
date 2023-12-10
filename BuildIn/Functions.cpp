#include "Functions.h"

namespace miniT
{
	std::unordered_map<std::string, Function*> Functions::BuildIn = {
		{"sin", new BuildInSin()},
		{"cos", new BuildInCos()},
		{"tan", new BuildInTan()},
		{"abs", new BuildInAbs()},
		{"exp", new BuildInExp()},
		{"log", new BuildInLog()},
		{"sqrt", new BuildInSqrt()}
	};
}