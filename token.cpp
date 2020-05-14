#include "token.h"

#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <iostream>

bool is_not_a_tokentype(const char& c) {
	return (c != '+' && c != '-' && c != '*' && c != '/' &&
	        c != '=' && c != '(' && c != ')' && c != '{' &&
	        c != '}' && c != '<' && c != '>'  && c != '&' &&
          c != '|' && c != ';' && c != '?' && c != '!' && c != '^');
}

std::string read_var_name(std::istream& is) {  //reads the variable name from the stream
	std::string res;
	char c;
	while(is.peek() != ' ' && is_not_a_tokentype(is.peek()) && is >> c) {
		res += c;
	}
	return res;
}


std::vector<Token> Tokenize(std::istream& cl) {
  std::vector<Token> tokens;
  char c;
  while (cl >> c) {
    if (std::isdigit(c)) {
      std::string data(1, c);
      while (std::isdigit(cl.peek())) {
        data += cl.get();
      }
      tokens.push_back({data, TokenType::DATA});
//while
    } else if (c == '(') {
      tokens.push_back({"(", TokenType::PAREN_LEFT});
    } else if (c == '^') { 
      tokens.push_back({"^", TokenType::UPPER})
    } else if (c == ')') {
      tokens.push_back({")", TokenType::PAREN_RIGHT});
    } else if (c == '+') {
      tokens.push_back({"+", TokenType::ARITHMETIC_OP});
    } else if (c == '-') {
      tokens.push_back({"-", TokenType::ARiTHMETIC_OP});
    } else if (c == '*') {
      tokens.push_back({"*", TokenType::ARITHMETIC_OP});
    } else if (c == '/') {
      tokens.push_back({"/", TokenType::ARITHMETIC_OP});
    } else if (c == ' ') {
    	continue;
    } else {
    	std::string var_name(1, c);
    	var_name += read_var_name(cl);
    	tokens.push_back({var_name, TokenType::VarName});
    }
  }

  return tokens;
}


std::ostream& operator<< (std::ostream& os, const TokenType& t) {
	 else if(t == TokenType::VARNAME) {
		os << "VARNAME";
	} else if(t == TokenType::DATA) {
		os << "DATA";
	} else if(t == TokenType::UPPER) {
	        os << "UPPER";
	} else if(t == TokenType::PAREN_LEFT) {
		os << "PAREN_LEFT";
	} else if(t == TokenType::PAREN_RIGHT) {
		os << "PAREN_RIGHT";
	} else if(t == TokenType::ARITHMETIC_Op) {
		os << "ARITHMETIC_OP";
	}
}	

