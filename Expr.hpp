#pragma once

#include <string>
#include "token.h"
using namespace std;

class Mult;
class Expr;

class Node {
public:
	virtual Expr dif() = 0;
	virtual std::string print() const = 0;
	virtual ~Node() {}
};

class Expr : public Node {
public:
	Expr();
	explicit Expr(const vector<Node*>& exp);
	explicit Expr(Node* exp);

	void Add(const Expr& exp);

	vector<Node*> vars;

	Expr dif() override;

	std::string print() const override;
};


class Data final : public Node {
	const std::string data;
	Data* D;
public:
	Data(const std::string& dat);

	Expr dif() override;

	std::string print() const override;

	~Data();
};


class Variable final : public Node {
	Data* D;
public:
	Variable();
	Expr dif() override;

	std::string print() const override;

	~Variable();
};


class Plus final : public Node {
	Expr left_, right_;
	Node* sum;
public:
	explicit Plus(const Expr& lhs, const Expr& rhs);

	Expr dif() override;

	std::string print() const override;

	void init();

	~Plus();
};


class Minus final : public Node {
	Expr left_, right_;
	Node* sum;
public:
	explicit Minus(const Expr& lhs, const Expr& rhs);
	Expr dif() override;

	std::string print() const override;

	void init();

	~Minus();
};


class Mult final : public Node {
	Expr left_, right_;
	Node* lf, * rg;
	Node* sum;
public:
	explicit Mult(const Expr& lhs, const Expr& rhs);

	Expr dif() override;

	std::string print() const override;

	void init();

	~Mult();
};

class Division final : public Node {
	Expr left_, right_;
	Node* lf, * rg, * min;
	Node* div;
	std::string pow;
public:
	explicit Division(const Expr& lhs, const Expr& rhs, const std::string& pw = "");

	Expr dif() override;

	std::string print() const override;

	void init();

	~Division();
};


class Pown final : public Node {
	Expr exp_;
	int power;
	Node* coef, * mul1, * mul2, * pwr;
public:
	explicit Pown(const Expr& exp, const int pw);
	Expr dif() override;

	std::string print() const override;

	~Pown();
};


class Cos final : public Node {
	Expr exp_;
	Node* sin_, * mul;
public:
	explicit Cos(const Expr& exp);
	Expr dif() override;

	std::string print() const override;

	~Cos();
};

class Sin final : public Node {
	Expr exp_;
	Node* cos, * mul;
	std::string minus;
public:
	explicit Sin(const Expr& exp, const std::string& m = "");
	Expr dif() override;

	std::string print() const override;

	~Sin();
};


class Expon final : public Node {
	Expr exp_;
	Node* expon, * mul;
	std::string minus;
public:
	explicit Expon(const Expr& exp);
	Expr dif() override;

	std::string print() const override;

	~Expon();
};

class Log final : public Node {
	Expr exp_;
	Node* dat, * div;
	std::string minus;
public:
	explicit Log(const Expr& exp);
	Expr dif() override;

	std::string print() const override;

	~Log();
};