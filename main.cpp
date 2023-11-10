#include <iostream>
 
#include "Tokenizer.h"
#include "TreeNodes.h"
#include "Parser.h"

int main() {

	std::cout << "Gebe bitte einen Term ein: ";

	std::string userInput;

	std::getline(std::cin, userInput);

	std::cout << userInput << "\n";

	miniT::Tokenizer tk = miniT::Tokenizer(userInput);

	miniT::Parser parser = miniT::Parser(&tk);

	miniT::TreeNode* tree = parser.GetTree();

	std::cout << tree->ToString() << " = " << tree->Eval() << "\n";

	return -1;
}
