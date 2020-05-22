#pragma once

#include <memory>
#include <map>
#include <stack>
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
	Expr() = default;
	explicit Expr(const vector<Node*>& exp) : vars(exp) {}
	explicit Expr(Node* exp) { vars.push_back(exp); }
	void Add(const Expr& exp) { vars.insert(vars.end(), exp.vars.begin(), exp.vars.end()); }

	vector<Node*> vars;

	Expr dif() override {
		Expr res;
		for (auto&& i : vars) {
			res.Add(i->dif());
		}
		return res;
	}

	std::string print() const override {
		std::string res;
		for (auto&& i : vars) {
			res += i->print();
		}
		return res;
	}
};


class Data final : public Node {
	const std::string data;
	Data* D;
public:
	Data(const std::string& dat) : data(dat) {}
	Expr dif() override {
		D = new Data{ "0" };
		return Expr{ D };
	}

	std::string print() const override {
		return data;
	}

	~Data() { delete D; }
};


class Variable final : public Node {
	Data* D;
public:
	Variable() {}
	Expr dif() override {
		D = new Data{ "1" };
		return Expr{ D };
	}

	std::string print() const override {
		return "x";
	}

	~Variable() { delete D; }
};


class Plus final : public Node {
	Expr left_, right_;
	Node* sum;
public:
	explicit Plus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}
	explicit Plus(const vector<Node*>& lhs, const vector<Node*>& rhs) :
		left_(Expr{ lhs }), right_(Expr{ rhs }) {}

	Expr dif()  override {
		init();
		return Expr{ sum };
	}

	std::string print() const override {
		return left_.print() + "+" + right_.print();
	}

	void init() {
		Expr lhs = left_.dif(), rhs = right_.dif();
		if (lhs.print() == "0" && rhs.print() == "0") {
			sum = sum = new Data{ "0" };
		} else if (rhs.print() == "0") {
			sum = new Expr{ lhs };
		} else if (lhs.print() == "0") {
			sum = new Expr{ rhs };
		} else {
			sum = new Plus{ lhs, rhs };
		}
	}

	~Plus() { delete sum; }
};


class Minus final : public Node {
	Expr left_, right_;
	Node* sum;
public:
	explicit Minus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}
	explicit Minus(const vector<Node*>& lhs, const vector<Node*>& rhs) :
		left_(Expr{ lhs }), right_(Expr{ rhs }) {}
	Expr dif() override {
		init();
		return Expr{ sum };
	}

	std::string print() const override {
		return left_.print() + "-(" + right_.print() + ")";
	}

	void init() {
		Expr lhs = left_.dif(), rhs = right_.dif();
		if (lhs.print() == "0" && rhs.print() == "0") {
			sum = sum = new Data{ "0" };
		} else if (rhs.print() == "0") {
			sum = new Expr{ lhs };
		} else if (lhs.print() == "0") {
			sum = new Expr{ rhs };
		} else {
			sum = new Minus{ lhs, rhs };
		}
	}

	~Minus() { delete sum; }
};


class Mult final : public Node {
	Expr left_, right_;
	Node* lf, * rg;
	Node* sum;
public:
	explicit Mult(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}

	Expr dif() override {
		init();
		return Expr{ sum };
	}

	std::string print() const override {
		if (left_.print().size() == 1 && right_.print().size() == 1)
			return left_.print() + "*" + right_.print();
		if (left_.print().size() == 1)
			return left_.print() + "*(" + right_.print() + ")";
		if (right_.print().size() == 1)
			return "(" + left_.print() + ")*" + right_.print();
		return "(" + left_.print() + ")*(" + right_.print() + ")";
	}

	void init() {
		Expr lhs = left_.dif(), rhs = right_.dif();
		if (rhs.print() == "0" && lhs.print() == "0") {
			sum = new Data{ "0" };
		}
		else if (rhs.print() == "0") {
			if (lhs.print() == "1") {
				sum = new Expr{ right_ };
			}
			else if (right_.print() == "1") {
				sum = new Expr{ lhs };
			}
			else {
				sum = new Mult{ lhs, right_ };
			}
		}
		else if (lhs.print() == "0") {
			if (rhs.print() == "1") {
				sum = new Expr{ left_ };
			}
			else if (left_.print() == "1") {
				sum = new Expr{ rhs };
			}
			else {
				sum = new Mult{ left_, rhs };
			}
		}
		else {
			if (lhs.print() == "1") {
				rg = new Expr{ right_ };
			}
			else if (right_.print() == "1") {
				rg = new Expr{ lhs };
			}
			else {
				rg = new Mult{ lhs, right_ };
			}

			if (rhs.print() == "1") {
				lf = new Expr{ left_ };
			}
			else if (left_.print() == "1") {
				lf = new Expr{ rhs };
			}
			else {
				lf = new Mult{ left_, rhs };
			}

			sum = new Plus{ Expr{lf}, Expr{rg} };
		}
	}

	~Mult() { delete lf; delete rg; delete sum; }
};

class Division final : public Node {
	Expr left_, right_;
	Node *lf, *rg, *min;
	Node *div;
public:
	explicit Division(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}

	Expr dif() override {
		init();
		return Expr{ div };
	}

	std::string print() const override {
		return "(" + left_.print() + ")/(" + right_.print() + ")^2 ";
	}

	void init() {
		Expr lhs = left_.dif(), rhs = right_.dif();
		if (right_.print() == "0") {
			throw std::logic_error("Division by zero");
		}
		if (rhs.print() == "0" && lhs.print() == "0") {
			min = new Data{ "0" };
		}
		else if (rhs.print() == "0") {
			if (lhs.print() == "1") {
				min = new Expr{ right_ };
			}
			else if (right_.print() == "1") {
				min = new Expr{ lhs };
			}
			else {
				min = new Mult{ lhs, right_ };
			}
		}
		else if (lhs.print() == "0") {
			if (rhs.print() == "1") {
				lf = new Data{ "-1" };
				min = new Mult{ Expr{lf}, Expr{ left_ } };
				//min = new Expr{ left_ };
			}
			else if (left_.print() == "1") {
				lf = new Data{ "-1" };
				min = new Mult{ Expr{lf}, Expr{ rhs } };
			}
			else {
				lf = new Data{ "-1" };
				rg = new Mult{ left_, rhs };
				min = new Mult{ Expr{lf}, Expr{rg} };
			}
		}
		else {
			if (lhs.print() == "1") {
				rg = new Expr{ right_ };
			}
			else if (right_.print() == "1") {
				rg = new Expr{ lhs };
			}
			else {
				rg = new Mult{ lhs, right_ };
			}

			if (rhs.print() == "1") {
				lf = new Expr{ left_ };
			}
			else if (left_.print() == "1") {
				lf = new Expr{ rhs };
			}
			else {
				lf = new Mult{ left_, rhs };
			}

			min = new Minus{ Expr{rg}, Expr{lf} };
		}
		div = new Division{ Expr{min}, right_ };
	}

	~Division() { delete lf; delete rg; delete div; }
};


class Pown final : public Node {
	Expr exp_;
	int power;
	Node* coef, *mul1, *mul2, *pwr;
public:
	explicit Pown(const Expr& exp, const int pw) : exp_(exp), power(pw) {}
	Expr dif() override {
		if (power == 0) {
			coef = new Data{ "0" };
			return Expr{ coef };
		}
		else if (power == 1) {
			return exp_.dif();
		}
		else {
			Expr exp_d = exp_.dif();
		  coef = new Data{ to_string(power) };
			pwr = new Pown{ exp_, power - 1 };
			mul1 = new Mult{ Expr{coef}, Expr{pwr} };
			if (exp_d.print() == "1") {
				return Expr{mul1};
			} 
			mul2 = new Mult{ Expr{mul1}, Expr{exp_d} };
			return Expr{mul2};
		}
	}

	std::string print() const override {
		if (power == 1) {
			return  exp_.print();
		}
		if (exp_.print() == "x")
		  return  "x^(" + to_string(power) + ")";
		return  "(" + exp_.print() + ")^(" + to_string(power) + ")";
	}

	~Pown() { delete mul1; delete mul2; delete pwr; }
};


class Cos final : public Node{
	Expr exp_;
	Node* sin_, * mul;
public:
	explicit Cos(const Expr& exp) : exp_(exp) {}
	Expr dif() override;

	std::string print() const override {
		return "cos(" + exp_.print() + ")";
	}

	~Cos() { delete sin_; delete mul; }
};

class Sin final : public Node {
	Expr exp_;
	Node* cos, * mul;
	std::string minus;
public:
	explicit Sin(const Expr& exp, const std::string& m = "") : exp_(exp), minus(m) {}
  Expr dif() override{
		Expr exp_d = exp_.dif();
		cos = new Cos(exp_);
		if (exp_d.print() == "1")
			mul = new Expr{cos};
		else 
			mul = new Mult{ Expr{cos}, exp_d };
		return Expr{mul};
	}

	std::string print() const override {
		return minus + "sin(" + exp_.print() + ")";
	}

	~Sin() { delete cos; delete mul; }
};

Expr Cos::dif() {
	Expr exp_d = exp_.dif();
	sin_ = new Sin{ exp_, "-" };
	if (exp_d.print() == "1")
		mul = new Expr{ sin_ };
	else
	  mul = new Mult{ Expr{sin_}, exp_.dif() };
	return Expr{ mul };
}
