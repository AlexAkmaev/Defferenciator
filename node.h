#pragma once

#include <memory>
#include <map>
#include <stack>
#include <string>
#include "token.h"


class Node {
public:
  virtual std::string Evaluate() const = 0;
  virtual std::string self() const = 0;
};

class Variable : public Node {  //class for variables and numbers
public:
  explicit Variable(std::string x);

  std::string Evaluate() const override;  //the contents of this variable or the value of a number
  std::string self() const override;

private:
  std::string _x;
};

class Op : public Node {  //class for operations
public:
  explicit Op(std::string val);

  const uint8_t precedence;  //priority of the operation

  std::string Evaluate() const override;  //calculating an expression based on the values of the left and right parts
  std::string self() const override;

  void SetLeft(std::shared_ptr<Node> node);  //value to the left of the operand
  void SetRight(std::shared_ptr<Node> node);  //value to the right of the operand

private:
  std::string _op;
  std::shared_ptr<const Node> _left, _right;
};
