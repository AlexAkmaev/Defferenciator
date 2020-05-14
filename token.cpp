#include "token.h"

#include <sstream>
#include <cstdio>
#include <stdexcept>
#include <iostream>

bool is_not_a_tokentype(const char& c) {
	return (c != '+' && c != '-' && c != '*' && c != '/' &&
	        c != '=' && c != '(' && c != ')' && c != '{' &&
	        c != '}' && c != '<' && c != '>'  && c != '&' &&
          c != '|' && c != ';' && c != '?' && c != '!');
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
      tokens.push_back({data, TokenType::Data});
//while
    } else if (c == 'w') {
    	std::string s = "w";
    	if ((s += cl.get()) == "wh" && (s += cl.get()) == "whi" && (s += cl.get()) == "whil" && (s += cl.get()) == "while") {
        tokens.push_back({s, TokenType::While});
      } else {
      	std::string var_name;
      	var_name = s;
        var_name += read_var_name(cl);
        tokens.push_back({var_name, TokenType::VarName});
      }
//if      
    } else if (c == 'i') {
    	std::string s = "i";
    	if ((s += cl.get()) == "if") {
        tokens.push_back({s, TokenType::If});
      } else {
      	std::string var_name;
        var_name = s;
        var_name += read_var_name(cl);
        tokens.push_back({var_name, TokenType::VarName});
      }
//print
    } else if (c == 'p') {
    	std::string s(1, c);
    	if ((s += cl.get()) == "pr" && (s += cl.get()) == "pri" && (s += cl.get()) == "prin" && (s += cl.get()) == "print") {
        tokens.push_back({s, TokenType::Print});
	    } else {
	    	std::string var_name;
	    	var_name = s;
	      var_name += read_var_name(cl);
	      tokens.push_back({var_name, TokenType::VarName});
	    }
//ENDL
		} else if (c == 'E') {
    	std::string s(1, c);
    	if ((s += cl.get()) == "EN" && (s += cl.get()) == "END" && (s += cl.get()) == "ENDL") {
        tokens.push_back({s, TokenType::Endl});
	    } else {
	    	std::string var_name;
	    	var_name = s;
	      var_name += read_var_name(cl);
	      tokens.push_back({var_name, TokenType::VarName});
	    }
		} else if (c == '"') {
    	std::string str;
    	std::getline(cl, str, '"');
    	tokens.push_back({str, TokenType::String});
		} else if (c == '?') {
    	tokens.push_back({"?", TokenType::Scanf});
 	  } else if (c == ';') {
    	tokens.push_back({";", TokenType::Semicolon});
 	  } else if (c == '(') {
      tokens.push_back({"(", TokenType::Paren_LEFT});
    } else if (c == ')') {
      tokens.push_back({")", TokenType::Paren_RIGHT});
    } else if (c == '{') {
      tokens.push_back({"{", TokenType::Bracelet_LEFT});
    } else if (c == '}') {
      tokens.push_back({"}", TokenType::Bracelet_RIGHT});
    } else if (c == '<') {
      if (cl.peek() == '=') {
        cl.get();
        tokens.push_back({"<=", TokenType::Compare_Op});
      } else {
        tokens.push_back({"<", TokenType::Compare_Op});
      }
    } else if (c == '>') {
      if (cl.peek() == '=') {
        cl.get();
        tokens.push_back({">=", TokenType::Compare_Op});
      } else {
        tokens.push_back({">", TokenType::Compare_Op});
      }
    } else if (c == '=') {
      if (cl.peek() == '=') {
      	cl.get();
        tokens.push_back({"==", TokenType::Compare_Op});
      } else {
        tokens.push_back({"=", TokenType::Assign});
      }
    } else if (c == '!') {
      if (cl.get() == '=') {
        tokens.push_back({"!=", TokenType::Compare_Op});
      } else {
        throw std::logic_error("Unknown token 1");
      }
    } else if (c == '&') {
      if (cl.get() == '&') {
        tokens.push_back({"&&", TokenType::Compare_Op});
      } else {
        throw std::logic_error("Unknown token 2");
      }
    } else if (c == '|') {
      if (cl.get() == '|') {
        tokens.push_back({"||", TokenType::Compare_Op});
      } else {
        throw std::logic_error("Unknown token 3");
      }
    } else if (c == '+') {
      tokens.push_back({"+", TokenType::Arithmetic_Op});
    } else if (c == '-') {
      tokens.push_back({"-", TokenType::Arithmetic_Op});
    } else if (c == '*') {
      tokens.push_back({"*", TokenType::Arithmetic_Op});
    } else if (c == '/') {
      tokens.push_back({"/", TokenType::Arithmetic_Op});
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
	 else if(t == TokenType::VarName) {
		os << "VARMANE";
	} else if(t == TokenType::Data) {
		os << "DATA";
	} else if(t == TokenType::Paren_LEFT) {
		os << "Paren_LEFT";
	} else if(t == TokenType::Paren_RIGHT) {
		os << "Paren_RIGHT";
	} else if(t == TokenType::Bracelet_LEFT) {
		os << "Bracelet_LEFT";
	} else if(t == TokenType::Bracelet_RIGHT) {
		os << "Bracelet_RIGHT";
	} else if(t == TokenType::Arithmetic_Op) {
		os << "ARIFMETIC_OP";
	}
}	

