#include "block.h"
#include "token.h"

/* token_parsed_file *
	To print a parsed file for tokens, pass the ParFile keyword
	as the third parameter to the command line parameters.
*/
void token_parsed_file(const std::vector<Token>& tokens) {
	for(const auto& i : tokens) {
		std::cout << i.value << "  ";
		if (i.type == TokenType::Semicolon) {
	    std::cout << std::endl;
		}
	}
}

int main(int argc, char *argv[]) {
	
	std::fstream is;
	is.open(argv[1], std::ios::in);
	
	std::vector<Token> tokens = Tokenize(is);
	
	if (argc == 3 && static_cast<std::string>(argv[2]) == "ParFile") {
	  token_parsed_file(tokens);
  }
  
	std::map<std::string, int> name_value;
	Block main_block{name_value};
	main_block.Execution(tokens);
	
	is.close();
	return 0;
}
