#include <fstream>
#include "Expr.hpp"

/* token_parsed_file *
	To print a parsed file for tokens, pass the PF keyword
	as the third parameter to the command line parameters.
*/
void token_parsed_file(const std::vector<Token>& tokens) {
	for (const auto& i : tokens) {
		std::cout << i.value << "  ";
	}
	std::cout << std::endl;
}

/*  find_pair_bracket *
	First, select the type of closing bracket.
	Then it counts the number of opening brackets of this type
	and subtracts the number of closing brackets.
	Returns an iterator to the found closing bracket.
*/
template <typename Iterator>
Iterator find_pair_bracket(Iterator beg_, Iterator end_) {
	int depth = 1;
	const TokenType& closing_bracket = TokenType::PAREN_RIGHT,
		bracket_type = TokenType::PAREN_LEFT;
	for (Iterator it = beg_; it != end_; ++it) {
		if (it->type == bracket_type) {
			++depth;
		}
		else if (it->type == closing_bracket) {
			--depth;
		}
		if (depth < 0)
			throw std::logic_error("Incorrect bracket sequence");
		if (depth == 0)
			return it;
	}

	return end_;
}

template <typename Iterator>
Iterator find_op(Iterator beg_, Iterator end_) {
	if (beg_ == end_) return end_;
	for (Iterator it = beg_; it != end_; ++it) {
		if (it->type == TokenType::ARITHMETIC_OP)
			return it;
	}
	return end_;
}

template <typename Iterator>
Expr make_expr(Iterator beg_, Iterator end_) {
	Expr res;
	for (Iterator token = beg_; token != end_; ++token) {
		if (token->type == TokenType::VARNAME) {
			Variable* var = new Variable{};
			res.Add(Expr{ var });
		} else if (token->type == TokenType::DATA) {
			Data* dat = new Data{ token->value };
			res.Add(Expr{ dat });
		}
	}
	return res;
}

Expr Expressionize(const vector<Expr>& null_prec, const vector<Token>& ops1) {
	Expr res;
	vector<Expr> first_prec;
	vector<Token> ops2;
	int i = 0;
	Expr cur = null_prec[i];
	for (auto&& op : ops1) {
		++i;
		if (op.value == "*") {
			Mult* mul = new Mult{ cur, null_prec[i] };
			cur = Expr{ mul };
		} else if (op.value == "/") {
			Division* div = new Division{ cur, null_prec[i] };
			cur = Expr{ div };
		} else if (op.value == "+" || op.value == "-") {
			ops2.push_back(op);
			first_prec.push_back(cur);
			cur = null_prec[i];
		}
	}
	first_prec.push_back(cur);

	i = 0;
	res.Add(first_prec[i]);
	for (auto&& op : ops2) {
		++i;
		if (op.value == "+") {
			Plus* pl = new Plus(res, first_prec[i]);
			res = Expr{ pl };
		}
		else if (op.value == "-") {
			Minus* ms = new Minus(res, first_prec[i]);
			res = Expr{ ms };
		}
	}

	return res;
}

//template <typename Iterator>
//Expr make_first_precedence(Iterator beg_, Iterator end_) {
//	Expr res;
//	vector<Token> ops1;
//	vector<Expr> null_prec;
//	Iterator op = find_op(beg_, end_);
//	for (Iterator token = beg_; token != end_; ++token) {
//		null_prec.push_back(make_expr(token, op));
//		token = op;
//		if (op == end_) { break; }
//		ops1.push_back(*op);
//		op = find_op(op + 1, end_);
//	}
//	std::cout << "< 1 >" << ops1.size() << "\n";
//	
//	res = Expressionize(null_prec, ops1);
//	return res;
//}

template <typename Iterator>
Expr recognize(Iterator it_beg, Iterator it_end) {
	Expr res;
	vector<Token> ops1;
	vector<Expr> null_prec;
	Iterator op = find_op(it_beg, it_end);
	for(auto token = it_beg; token != it_end; ++token) {
		if(token->type == TokenType::PAREN_LEFT) {
			auto paren_right = find_pair_bracket(token + 1, it_end);
			null_prec.push_back(recognize(token + 1, paren_right));
			token = paren_right + 1;
			if (token == it_end) break;
			if (token->type == TokenType::ARITHMETIC_OP)
			  ops1.push_back(*token);
			op = find_op(token + 1, it_end);
		} else if (token->type == TokenType::COS || token->type == TokenType::SIN) {
			if ((token + 1)->type != TokenType::PAREN_LEFT)
				throw std::logic_error("Need left brace");
			auto paren_right = find_pair_bracket(token + 2, it_end);
			Node* in_exp;
			if (token->type == TokenType::SIN) {
				in_exp = new Sin{ recognize(token + 2, paren_right) };
			} else {
				in_exp = new Cos{ recognize(token + 2, paren_right) };
			}
			token = paren_right + 1;
			null_prec.push_back(Expr{in_exp});
			if (token == it_end) break;
			if (token->type == TokenType::ARITHMETIC_OP)
				ops1.push_back(*token);
			op = find_op(token + 1, it_end);
		} else {
			null_prec.push_back(make_expr(token, op));
			token = op;
			if (op == it_end) { break; }
			ops1.push_back(*op);
			op = find_op(op + 1, it_end);
		}
	}

	res = Expressionize(null_prec, ops1);
	return res; //make_first_precedence(it_beg, it_end);
}


int main(int argc, char* argv[]) {

	std::vector<Token> tokens;

	if (argc > 1) {
		std::fstream is;
		is.open(argv[1], std::ios::in);

		tokens = Tokenize(is);

		is.close();
	} else {
		tokens = Tokenize(std::cin);
	}

#ifdef PF
	token_parsed_file(tokens);
#endif

	Expr res = recognize(tokens.begin(), tokens.end());
	Expr ans = res.dif();
	std::cout << ans.print();

	return 0;
}
