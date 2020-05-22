#include "Expr.hpp"

/**Expr**/
Expr::Expr() = default;
Expr::Expr(const vector<Node*>& exp) : vars(exp) {}
Expr::Expr(Node* exp) { vars.push_back(exp); }

void Expr::Add(const Expr& exp) { vars.insert(vars.end(), exp.vars.begin(), exp.vars.end()); }

Expr Expr::dif() {
	Expr res;
	for (auto&& i : vars) {
		res.Add(i->dif());
	}
	return res;
}

std::string Expr::print() const {
	std::string res;
	for (auto&& i : vars) {
		res += i->print();
	}
	return res;
}


/**Data**/
Data::Data(const std::string& dat) : data(dat) {}
Expr Data::dif() {
	D = new Data{ "0" };
	return Expr{ D };
}

std::string Data::print() const {
	return data;
}

Data::~Data() { delete D; }


/**Variable**/
Variable::Variable() {}
Expr Variable::dif() {
	D = new Data{ "1" };
	return Expr{ D };
}

std::string Variable::print() const {
	return "x";
}

Variable::~Variable() { delete D; }


/**Plus**/
Plus::Plus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}

Expr Plus::dif() {
	init();
	return Expr{ sum };
}

std::string Plus::print() const {
	return left_.print() + "+" + right_.print();
}

void Plus::init() {
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

Plus::~Plus() { delete sum; }


/**Minus**/
Minus::Minus(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}

Expr Minus::dif() {
	init();
	return Expr{ sum };
}

std::string Minus::print() const {
	return left_.print() + "-(" + right_.print() + ")";
}

void Minus::init() {
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
		sum = new Minus{ lhs, rhs };
	}
}

Minus::~Minus() { delete sum; }


/**Mult**/
Mult::Mult(const Expr& lhs, const Expr& rhs) : left_(lhs), right_(rhs) {}

Expr Mult::dif() {
	init();
	return Expr{ sum };
}

std::string Mult::print() const {
	if (left_.print().size() == 1 && right_.print().size() == 1)
		return left_.print() + "*" + right_.print();
	if (left_.print().size() == 1)
		return left_.print() + "*(" + right_.print() + ")";
	if (right_.print().size() == 1)
		return "(" + left_.print() + ")*" + right_.print();
	return "(" + left_.print() + ")*(" + right_.print() + ")";
}

void Mult::init() {
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

Mult::~Mult() { delete lf; delete rg; delete sum; }


/**Division**/
Division::Division(const Expr& lhs, const Expr& rhs, const std::string& pw) :
	left_(lhs), right_(rhs), pow(pw) {}

Expr Division::dif() {
	init();
	return Expr{ div };
}

std::string Division::print() const {
	if (left_.print().size() == 1 && right_.print().size() == 1)
		return left_.print() + "/" + right_.print() + pow;
	if (left_.print().size() == 1)
		return left_.print() + "/(" + right_.print() + ")" + pow;
	if (right_.print().size() == 1)
		return "(" + left_.print() + ")/" + right_.print() + pow;
	return "(" + left_.print() + ")/(" + right_.print() + ")" + pow;
}

void Division::init() {
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
	div = new Division{ Expr{min}, right_, "^2 " };
}

Division::~Division() { delete lf; delete rg; delete div; }



/**Pown**/
Pown::Pown(const Expr& exp, const int pw) : exp_(exp), power(pw) {}
Expr Pown::dif() {
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
			return Expr{ mul1 };
		}
		mul2 = new Mult{ Expr{mul1}, Expr{exp_d} };
		return Expr{ mul2 };
	}
}

std::string Pown::print() const {
	if (power == 1) {
		return  exp_.print();
	}
	if (exp_.print() == "x")
		return  "x^(" + to_string(power) + ")";
	return  "(" + exp_.print() + ")^(" + to_string(power) + ")";
}

Pown::~Pown() { delete mul1; delete mul2; delete pwr; }


/**Cos**/
Cos::Cos(const Expr& exp) : exp_(exp) {}

Expr Cos::dif() {
	Expr exp_d = exp_.dif();
	sin_ = new Sin{ exp_, "-" };
	if (exp_d.print() == "1")
		mul = new Expr{ sin_ };
	else
		mul = new Mult{ Expr{sin_}, exp_.dif() };
	return Expr{ mul };
}

std::string Cos::print() const {
	return "cos(" + exp_.print() + ")";
}

Cos::~Cos() { delete sin_; delete mul; }


/**Sin**/
Sin::Sin(const Expr& exp, const std::string& m) : exp_(exp), minus(m) {}

Expr Sin::dif() {
	Expr exp_d = exp_.dif();
	cos = new Cos(exp_);
	if (exp_d.print() == "1")
		mul = new Expr{ cos };
	else
		mul = new Mult{ Expr{cos}, exp_d };
	return Expr{ mul };
}

std::string Sin::print() const {
	return minus + "sin(" + exp_.print() + ")";
}

Sin::~Sin() { delete cos; delete mul; }


/**Exp**/
Expon::Expon(const Expr& exp) : exp_(exp) {}

Expr Expon::dif() {
	Expr exp_d = exp_.dif();
	expon = new Expon{ exp_ };
	if (exp_d.print() == "1")
		mul = new Expr{ expon };
	else
		mul = new Mult{ Expr{expon}, exp_d };
	return Expr{ mul };
}

std::string Expon::print() const {
	return "exp(" + exp_.print() + ")";
}

Expon::~Expon() { delete expon; delete mul; }

/**Log**/
Log::Log(const Expr& exp) : exp_(exp) {}

Expr Log::dif() {
	Expr exp_d = exp_.dif();
	if (exp_d.print() == "1")
		dat = new Data{ "1" };
	else
		dat = new Expr{exp_d};
	div = new Division{ Expr{dat}, exp_ };
	return Expr{ div };
}

std::string Log::print() const {
	return "ln(" + exp_.print() + ")";
}

Log::~Log() { delete dat; delete div; }