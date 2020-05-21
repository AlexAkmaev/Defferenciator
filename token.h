#pragma once

#include <sstream>
#include <vector>
#include <iostream>
#include <string>

enum class TokenType {
	ARITHMETIC_OP,
	VARNAME,
	DATA,
	UPPER,
	PAREN_LEFT,
	PAREN_RIGHT,
	COS,
	SIN,
};

struct Token {
	const std::string value;
	const TokenType type;
};

std::vector<Token> Tokenize(std::istream& is);  //splits the file into tokens
std::ostream& operator<< (std::ostream& os, const TokenType& t);  //output operator for the token