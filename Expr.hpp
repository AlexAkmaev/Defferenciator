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
	//	explicit Expr(const Node& func, const vector<Node>& exp) {
	//		Expr a{func}, b{exp};
	//		Mult m{ a, b};
	//	  vars.push_back(m);
	////	  vars.push_back(Mult{ Expr{func}, Expr{exp} });
	//	}

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
		}
		else if (rhs.print() == "0") {
			sum = new Expr{ lhs };
		}
		else if (lhs.print() == "0") {
			sum = new Expr{ rhs };
		}
		else {
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
		return left_.print() + "*" + right_.print();
	}

	void init() {
		Expr lhs = left_.dif(), rhs = right_.dif();
		if (rhs.print() == "0" && lhs.print() == "0") {
			sum = new Data{ "0" };
		} else if (rhs.print() == "0") {
			if (lhs.print() == "1") {
				sum = new Expr{ right_ };
			} else if (right_.print() == "1") {
				sum = new Expr{ lhs };
			} else {
				sum = new Mult{ lhs, right_ };
			}
		} else if (lhs.print() == "0") {
			if (rhs.print() == "1") {
				sum = new Expr{ left_ };
			} else if (left_.print() == "1") {
				sum = new Expr{ rhs };
			} else {
				sum = new Mult{ left_, rhs };
			}
		} else {
			if (lhs.print() == "1") {
				rg = new Expr{ right_ };
			} else if (right_.print() == "1") {
				rg = new Expr{ lhs };
			} else {
				rg = new Mult{ lhs, right_ };
			}

			if (rhs.print() == "1") {
				lf = new Expr{ left_ };
			} else if (left_.print() == "1") {
				lf = new Expr{ rhs };
			} else {
				lf = new Mult{ left_, rhs };
			}

			sum = new Plus{ Expr{lf}, Expr{rg} };
		}
	}

	~Mult() { delete lf; delete rg; delete sum; }
};




//class Mult;
//class Expr;
//
//class Node {
//public:
//	//const std::string value;
//	virtual Expr dif() const = 0;
//	virtual void print() const = 0;// { std::cout << " EMPPRINT "; }
//	//virtual ~Node() {}
//};
//
//class Expr : public Node {
//public:
//	Expr() = default;
//	explicit Expr(const vector<Node*>& exp) : vars(exp) {}
//	//	explicit Expr(const Expr& lhs, const Expr& rhs) { vars.push_back()}
//	explicit Expr(Node* exp) { vars.push_back(exp); }
//	void Add(const Expr& exp) { vars.insert(vars.end(), exp.vars.begin(), exp.vars.end()); }
//	//	explicit Expr(const Node& func, const vector<Node>& exp) {
//	//		Expr a{func}, b{exp};
//	//		Mult m{ a, b};
//	//	  vars.push_back(m);
//	////	  vars.push_back(Mult{ Expr{func}, Expr{exp} });
//	//	}
//
//	vector<Node*> vars;
//
//	Expr dif() const override {
//		//		vector<Node> ans;
//		Expr res;
//		for (auto&& i : vars) {
//			res.Add(i->dif());
//			//		  ans.push_back(i.dif());
//		}
//		return res;
//	}
//
//	void print() const override {
//		for (auto&& i : vars) {
//			i->print();
//			std::cout << " ";
//		}
//	}
//};
//
//
////Expr Node::dif() const  { std::cout << " EMPEXPR "; return Expr{}; }
//
//class Data final : public Node {
//	const std::string data;
//public:
//	Data(const std::string& dat) : data(dat) {}
//	Expr dif() const override {
//		return Expr{ &Data{data} };
//	}
//
//	void print() const override {
//		std::cout << data;
//	}
//};
//
//
//class Variable final : public Node {
//public:
//	Variable() {}
//	Expr dif() const override {
//		return Expr{ &Data{"1"} };
//	}
//
//	void print() const override {
//		std::cout << "x";
//	}
//};
//
//
//class Plus final : public Node {
//	Expr left_, right_;
//public:
//	explicit Plus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}
//	explicit Plus(const vector<Node*>& lhs, const vector<Node*>& rhs) :
//		left_(Expr{ lhs }), right_(Expr{ rhs }) {}
//	Expr dif() const override {
//		return Expr{ &Plus{left_.dif(), right_.dif()} };
//	}
//
//	void print() const override {
//		left_.print();
//		std::cout << "+";
//		right_.print();
//	}
//};
//
//
//class Minus final : public Node {
//	Expr left_, right_;
//public:
//	explicit Minus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}
//	explicit Minus(const vector<Node*>& lhs, const vector<Node*>& rhs) :
//		left_(Expr{ lhs }), right_(Expr{ rhs }) {}
//	Expr dif() const override {
//		return Expr{ &Minus{left_.dif(), right_.dif()} };
//	}
//
//	void print() const override {
//		left_.print();
//		std::cout << "+";
//		right_.print();
//	}
//};
//
//
//class Mult final : public Node {
//	Expr left_, right_;
//public:
//	explicit Mult(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}
//	Expr dif() const override {
//		return Expr{ &Plus{Expr{&Mult{left_, Expr{right_.dif()}}}, Expr{&Mult{Expr{left_.dif()}, right_}}} };
//	}
//
//	void print() const override {
//		left_.print();
//		std::cout << "*";
//		right_.print();
//	}
//};




//class Func : public Node{
//public:
//	virtual Expr dif() const = 0;
//};
//
//class Cos final : public Func{
//public:
//	explicit Cos(const Expr& exp) : exp_(exp) {}
//  Expr dif() const override{
//		return Expr(Sin(exp_), exp_.dif());
//	}
//private:
//	Expr exp_;
//};
//
//class Sin final : public Func{
//public:
//	explicit Sin(const Expr& exp) : exp_(exp) {}
//  Expr dif() const override{
//		return Expr(Cos(exp_), exp_.dif());
//	}
//private:
//	Expr exp_;
//};
