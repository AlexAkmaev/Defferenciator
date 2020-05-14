#include "block.h"

/*  find_pair_bracket *
	First, select the type of closing bracket.
	Then it counts the number of opening brackets of this type 
	and subtracts the number of closing brackets.
	Returns an iterator to the found closing bracket.
*/
template <typename Iterator>
Iterator find_pair_bracket(Iterator beg_, Iterator end_, const TokenType& bracket_type) {
	if ((bracket_type != TokenType::Paren_LEFT) && (bracket_type != TokenType::Bracelet_LEFT)) {
		throw std::logic_error("Unknown type of opening bracket");
	}
	int depth = 1;
	const TokenType& closing_bracket = bracket_type == TokenType::Paren_LEFT ?
                                     TokenType::Paren_RIGHT : TokenType::Bracelet_RIGHT;
	for(Iterator it = beg_; it != end_; ++it) {
		if (it->type == bracket_type) {
			++depth;
		} else if (it->type == closing_bracket) {
			--depth;
		}
		if (depth < 0)
		  throw std::logic_error("Incorrect bracket sequence");
    if (depth == 0) 
      return it;
	}
	
	return end_;
}

/*** class Block ***/
Block::Block(const std::map<std::string, int>& values) : name_value(values) {}

const std::map<std::string, int>& Block::map_names() { return name_value; }

void Block::Execution(const std::vector<Token>& tokens) {
	auto it_beg = tokens.begin(), it_end = tokens.end();
	for(auto token = it_beg; token != it_end; ++token) {
		if (token->type == TokenType::Assign) {  //if a token containing an assignment is encountered
			++token;
			if (token->type == TokenType::Scanf) {
				int value;
				std::cin >> value;
				std::string variable_name = (token - 2)->value;
				name_value[variable_name] = value;
			} else {
				auto semicolon = std::find(token, it_end, Token{";", TokenType::Semicolon});
				int value = Expression_evaluation(token, semicolon);
				std::string variable_name = (token - 2)->value;
				name_value[variable_name] = value;
		  }
		} else if (token->type == TokenType::Print) {  //if a token containing the print command is encountered
			++token;
			if (token->type == TokenType::Endl) {
				std::cout << std::endl;
			} else if (token->type == TokenType::String) {
				std::cout << token->value;
			} else {
				std::string variable_name = token->value;
				std::cout << name_value[variable_name];
			}
			//if a token containing a while loop command or an if statement is encountered
		} else if (token->type == TokenType::While || token->type == TokenType::If) { 
			const TokenType& keyword = token->type;
			++token;
			auto paren_right = find_pair_bracket(token + 1, it_end, TokenType::Paren_LEFT);
			if (token->type != TokenType::Paren_LEFT || paren_right == it_end)
			  throw std::logic_error("Wrong Syntax");
	    
	    auto bracelet_left = paren_right + 1;
	    auto bracelet_right = find_pair_bracket(bracelet_left + 1, it_end, TokenType::Bracelet_LEFT);
	    if (bracelet_left->type != TokenType::Bracelet_LEFT || bracelet_right == it_end)
			  throw std::logic_error("Wrong Syntax");
	    
			int condition;
			if (keyword == TokenType::While) {
		    while (condition = Expression_evaluation(token + 1, paren_right) ) {
		    	std::vector<Token> tokens_in_block(bracelet_left + 1, bracelet_right);
		    	Block While_block_of_code{name_value};
		    	While_block_of_code.Execution(tokens_in_block);
		    	const std::map<std::string, int>& new_values = While_block_of_code.map_names();
		    	for(auto& item : name_value) {
		    		item.second = new_values.at(item.first);
		    	}
		    }
		  } else {
		  	if (condition = Expression_evaluation(token + 1, paren_right) ) {
		    	std::vector<Token> tokens_in_block(bracelet_left + 1, bracelet_right);
		    	Block If_block_of_code{name_value};
		    	If_block_of_code.Execution(tokens_in_block);
		    	const std::map<std::string, int>& new_values = If_block_of_code.map_names();
		    	for(auto& item : name_value) {
		    		item.second = new_values.at(item.first);
		    	}
		    }
		  }
		  token = bracelet_right;
		}
	}
}
	
bool operator==(const Token& t1, const Token& t2) {
	return std::tie(t1.value, t1.type) == std::tie(t2.value, t2.type);
}
