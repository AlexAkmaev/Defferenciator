//#include "block.h"
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
			//res.print();
		}
		else if (token->type == TokenType::DATA) {
			Data* dat = new Data{ token->value };
			res.Add(Expr{ dat });
		}
	}
	return res;
}

template <typename Iterator>
Expr make_first_precedence(Iterator beg_, Iterator end_) {
	Expr res;
	vector<Token> ops1, ops2;
	vector<Expr> null_prec, first_prec;
	Iterator op = find_op(beg_, end_);
	for (Iterator token = beg_; token != end_; ++token) {
		null_prec.push_back(make_expr(token, op));
		token = op;
		if (op == end_) { break; }
			ops1.push_back(*op);
			op = find_op(op + 1, end_);
		
	}
	std::cout << "< 1 >" << ops1.size() << "\n";
	int i = 0;
	Expr cur = null_prec[i];
	for (auto&& op : ops1) {
		++i;
		if (op.value == "*") {
			Mult* mul = new Mult{ cur, null_prec[i] };
			cur = Expr{ mul };
		} else if (op.value == "+" || op.value == "-") {
			ops2.push_back(op);
			first_prec.push_back(cur);
			cur = null_prec[i];
		}
	}
	first_prec.push_back(cur);

	std::cout << "< 2 >" << ops2.size() << "\n";
	i = 0;
	res.Add(first_prec[i]);
	for (auto&& op : ops2) {
		++i;
		if (op.value == "+") {
			Plus* pl = new Plus(res, first_prec[i]);
			res = Expr{ pl };
		} else if (op.value == "-") {
			Minus* ms = new Minus(res, first_prec[i]);
			res = Expr{ ms };
		}
	}

	return res;
}

//template <typename Iterator>
//Expr make_first_precedence(Iterator beg_, Iterator end_) {
//	Expr res;
//	vector<Expr> first_prec;
//	vector<Token> ops;
//	Iterator op = find_op(beg_, end_);
//	Expr prev_exp = make_expr(beg_, op);
//	//first_prec.push_back(prev_exp);
//	Iterator prev_op = beg_;
//	//std::cout << prev_op->value << "||\n";
//	int k = 0;
//	for (Iterator token = op; token != end_; ++token) {
//		if ((op != end_) && op->value == "*") {
//			prev_exp = make_expr(prev_op, op);
//			token = find_op(op + 1, end_);
//			prev_op = (token == end_) ? end_ : token + 1;
//			Mult* mul = new Mult{ prev_exp, make_expr(op + 1, token) };
//			first_prec.push_back(Expr{ mul });
//		} else {
//			prev_exp = make_expr(prev_op, op);
//			first_prec.push_back(prev_exp);
//			if (op != end_) {
//				ops.push_back(*op);
//				prev_op = op + 1;
//			} else {
//				prev_op = end_;
//			}
//			token = op;
//		}
//		for (auto&& e : first_prec) {
//			e.print();
//		  std::cout << "| ";
//		}
//		std::cout << "<\n";
//		if (prev_op == end_) break;
//		op = find_op(prev_op, end_);
//		//std::cout << "|()|" << (prev_op == end_) << "\n";
//	}
//
//		//res.print();
//	std::cout << "<>" << ops.size() << "\n";
//	int i = 0;
//	res.Add(first_prec[i]);
//	for (auto&& tok : ops) {
//		++i;
//		if (tok.value == "+") {
//			Plus* pl = new Plus(res, first_prec[i]);
//			res = Expr{ pl };
//		} else if (tok.value == "-") {
//			Minus* ms = new Minus(res, first_prec[i]);
//			res = Expr{ ms };
//		}
//	}
//
//	return res;
//}



//template <typename Iterator>
//Expr make_expr(Iterator beg_, Iterator end_) {
//	Expr res;
//	for(Iterator token = beg_; token != end_; ++token) {
//		if(token->type == TokenType::VARNAME) {
//			res.Add(make_shared<Expr>(make_shared<Variable>()));
//			res.print();
//		} else if (token->type == TokenType::DATA) {
//			res.Add(make_shared<Expr>(make_shared<Data>(token->value)));
//		}
//	}
//	return res;
//}
//
//template <typename Iterator>
//Expr make_first_precedence(Iterator beg_, Iterator end_) {
//	Expr res;
//	vector<Expr> first_prec;
//	vector<Token> ops;
//  Iterator op = find_op(beg_, end_);
//	Expr prev_exp = make_expr(beg_, op);
//	Iterator prev_op = op;
//	for(Iterator token = op; token != end_; ++token) {
//		if(op->value == "*") {
//			prev_exp = make_expr(prev_op, op);
//			token = find_op(op + 1, end_);
//			prev_op = (token == end_) ? end_ : token + 1;
//			first_prec.push_back(Expr{
//			    make_shared<Mult>(make_shared<Expr>(prev_exp), make_shared<Expr>(make_expr(op + 1, token)))
//			            });
//		} else {
//			prev_exp = make_expr(prev_op, op);
//			first_prec.push_back(Expr{prev_exp});
//			ops.push_back(*op);
//			prev_op = op + 1;
//			token = op;
//		}
//		op = find_op(token + 1, end_);
//	}
//	
//	int i = 0;
//	res.Add(make_shared<Expr>(first_prec[i]));
//	for(auto&& tok : ops) {
//		if (tok.value == "+") {
//		  res = Expr{make_shared<Plus>(res, first_prec[i])};
//		} else if (tok.value == "-") {
//		  res = Expr{make_shared<Minus>(res, first_prec[i]) };
//		}
//    ++i;
//	}
//	
//	return res;
//}

Expr differenciate(const std::vector<Token>& tokens) {
	Expr res;
	auto it_beg = tokens.begin(), prev_op = it_beg, it_end = tokens.end();

	//	for(auto token = it_beg; token != it_end; ++token) {
	//		if(op->value == "*") {
	//			token = find_op(op + 1, it_end);
	//			res.Add(Expr{Mult{prev_exp, make_expr(op + 1, token)}});
	//		} else if(op->value == "+") {
	//			res.Add(make_expr(prev_op, op));
	//		} else if(op->value == "-") {
	//			res.Add(make_expr(prev_op, op));
	//		}
	//		op = find_op(token, it_end);
	//		//token = op;
	//	}
	return make_first_precedence(it_beg, it_end);
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

	Expr res = differenciate(tokens);
	Expr ans = res.dif();
	std::cout << ans.print();

	return 0;
}
