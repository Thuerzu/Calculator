#include "Constants.h"

namespace miniT
{
	std::unordered_map<std::string, Number*> Constants::BuiltIn = {
		{"PI", new Number(3.141592653589793)},
		{"E", new Number(2.718281828459045)}
	};
}