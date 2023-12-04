#include <iostream>
 
#include "Tokenizer.h"
#include "TreeNodes.h"
#include "Parser.h"
#include "Scope.h"

int main() {
	
	miniT::Scope stdScope = miniT::Scope();
	stdScope.Name = "std";
	stdScope.Add("pi", new miniT::Number(3.141592653589793));
	stdScope.Add("e",  new miniT::Number(2.718281828459045));

	std::cout << "===============PRESENTING: miniT Calculator==================\n";

	std::string userInput;

	std::getline(std::cin, userInput);

	std::cout << userInput << "\n";

	miniT::Tokenizer tk = miniT::Tokenizer(userInput);

	miniT::Parser parser = miniT::Parser(&tk, &stdScope);

	while (userInput != "exit")
	{
		tk.SetSource(userInput);
		parser.Parse();
		miniT::TreeNode* tree = parser.GetTree();
		std::cout << ">>>" << tree->ToString() << " = " << tree->Eval() << "\n";
		std::getline(std::cin, userInput);
	}

	return -1;
}
