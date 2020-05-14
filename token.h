#pragma once

#include <sstream>
#include <vector>
#include <iostream>
#include <string>

enum class TokenType {
	Logic_Op,
	Compare_Op,
	Arithmetic_Op,
	Assign,
	While,
	If,
	Print,
	Scanf,
	VarName,
	Data,
	Endl,
	String,
	Paren_LEFT,
	Paren_RIGHT,
	Bracelet_LEFT,
	Bracelet_RIGHT,
};

struct Token {
  const std::string value;
  const TokenType type;
};

std::vector<Token> Tokenize(std::istream& is);  //splits the file into tokens
std::ostream& operator<< (std::ostream& os, const TokenType& t);  //output operator for the token

