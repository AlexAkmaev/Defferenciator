#include "node.h"

/*** class Variable ***/
Variable::Variable(std::string x) : _x(x) {}

std::string Variable::Evaluate() const {
  if (_x == "cos") {
    return "-sin";
  } else if (_x == "sin") {
    return "cos";
  } else if (_x == "ln") {
    return "1/";
  } else if (_x == "exp") {
    return "exp";
  } else if (_x == "x") {
    return "x";
  } 
}

std::string Variable::self() const {
	return Evaluate();
}

/*** class Op ***/
Op::Op(std::string val)
    : precedence([val] {
        if (val == "*" || val == "/") {
          return 6;
        } else if (val == "+" || val == "-") {
          return 5;
      }()),
      _op(val) {}

std::string Op::Evaluate() const {
  if (_op == "*") {
    return _left->Evaluate() + "*" + _right->self + "+" + _left->self + "*" + _right->Evaluate();
  } else if (_op == "+") {
    return _left->Evaluate() + "+" + _right->Evaluate();
  } else if (_op == "-") {
    return _left->Evaluate() + "-" + _right->Evaluate();
  } else if (_op == "/") {
    return "(" + _left->Evaluate() + "*" + _right->self + "+" + _left->self + "*" + _right->Evaluate() +
		                 ")/(" + _right->Evaluate() + ")^2";
  return 0;
}

std::string Variable::self() const {
	return "|OP|";
}

void Op::SetLeft(std::shared_ptr<Node> node) { _left = node; }
void Op::SetRight(std::shared_ptr<Node> node) { _right = node; }

