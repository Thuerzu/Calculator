#include <iostream>
 
#include "Tokenizer.h"
#include "TreeNodes.h"
#include "Parser.h"
#include "Scope.h"
#include "BuildIn/Constants.h"
#include "BuildIn/Functions.h"

int main() {
	
	miniT::Scope stdScope = miniT::Scope();
	stdScope.Name = "std";
	miniT::Constants::AddToScope(&stdScope);
	miniT::Functions::AddToScope(&stdScope);

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
		if (tree)
		{
			try
			{
				std::cout << ">>>" << tree->ToString() << " = " << tree->Eval() << "\n";
			}
			catch (const std::exception& e)
			{
				std::cout << e.what() << "\n";
			}
		}
		std::getline(std::cin, userInput);
	}

	return -1;
}
