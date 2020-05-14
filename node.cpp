#include "node.h"

/*** class Variable ***/
Variable::Variable(int x) : _x(x) {}

int Variable::Evaluate() const { return _x; }

/*** class Op ***/
Op::Op(std::string val)
    : precedence([val] {
        if (val == "*" || val == "/") {
          return 6;
        } else if (val == "+" || val == "-") {
          return 5;
        } else if (val == "<" || val == "<=" || val == ">" || val == ">=") {
          return 4;
        } else if (val == "!=" || val == "==") {
          return 3;
        } else if (val == "&&") {
          return 2;
        } else {
          return 1;
        }
      }()),
      _op(val) {}

int Op::Evaluate() const {
  if (_op == "*") {
    return _left->Evaluate() * _right->Evaluate();
  } else if (_op == "+") {
    return _left->Evaluate() + _right->Evaluate();
  } else if (_op == "-") {
    return _left->Evaluate() - _right->Evaluate();
  } else if (_op == "/") {
    return _left->Evaluate() / _right->Evaluate();
  } else if (_op == "<") {
    return (_left->Evaluate() < _right->Evaluate());
  } else if (_op == "<=") {
    return (_left->Evaluate() <= _right->Evaluate());
  } else if (_op == ">") {
    return (_left->Evaluate() > _right->Evaluate());
  } else if (_op == ">=") {
    return (_left->Evaluate() >= _right->Evaluate());
  } else if (_op == "==") {
    return (_left->Evaluate() == _right->Evaluate());
  } else if (_op == "!=") {
    return (_left->Evaluate() != _right->Evaluate());
  } else if (_op == "&&") {
    return (_left->Evaluate() && _right->Evaluate());
  } else if (_op == "||") {
    return (_left->Evaluate() || _right->Evaluate());
  }
  return 0;
}

void Op::SetLeft(std::shared_ptr<Node> node) { _left = node; }
void Op::SetRight(std::shared_ptr<Node> node) { _right = node; }

