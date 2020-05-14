#pragma once

#include <sstream>
#include <algorithm>
#include <fstream>
#include <tuple>
#include "token.h"
#include "node.h"


/*** Shunting-yard algorithm ***
	It's a method for parsing mathematical expressions specified in infix notation. 
	It can produce either a postfix notation string, also known as Reverse Polish notation (RPN).
	An example of a simple conversion:
	1. Input: 3 + 4
	2. Push 3 to the output queue (whenever a number is read it is pushed to the output)
	3. Push + onto the operator stack
	4. Push 4 to the output queue
	5. After reading the expression, pop the operators with the highest precedence off the stack and add them to the output.
	6. In this case there is only one, "+".
	7. Output: 3 4 +
***/


//determines whether a minus should be added to an operation or assigned to a number
template <class Iterator>
bool is_negative_number(Iterator prev) {
	if (prev->type == TokenType::Data || prev->type == TokenType::VarName ||
		 prev->type == TokenType::Paren_RIGHT) {
		return false;
	}
	return true;
}

class Block {
public:	
  
	explicit Block(const std::map<std::string, int>& values);  //ctor

	const std::map<std::string, int>& map_names();  //returns a constant reference to the storage
	
	/*** Shunting-yard algorithm ***/
	template <class Iterator>
	int Expression_evaluation(Iterator beg, Iterator end) {
		Iterator token = beg;
		// Empty expression
	  if (token == end) {
	    return 0;
	  } else if (end - token == 1 && token->type == TokenType::Data) {
	  	return std::atoi(token->value.c_str());
	  }
	
	  std::stack<std::shared_ptr<Node>> values;
	  std::stack<std::shared_ptr<Op>> ops;
	  
	  auto PopOps = [&](int precedence) {
	    while (!ops.empty() && ops.top()->precedence >= precedence) {
	      auto value1 = values.top();
	      values.pop();
	      auto value2 = values.top();
	      values.pop();
	      auto op = ops.top();
	      ops.pop();
	
	      op->SetRight(value1);
	      op->SetLeft(value2);
	
	      values.push(op);
	    }
	  };
	  while (token != end) {
	    const auto &val = *token;
	    if (val.value == "*" || val.value == "/") {
	      PopOps(6);
	      ops.push(std::make_shared<Op>(val.value));
	    } else if (val.value == "+") {
	      PopOps(5);
	      ops.push(std::make_shared<Op>(val.value));
	    } else if (val.value == "-") {
	    	Iterator prev = token - 1, next = token + 1;
	    	if (!is_negative_number(prev)) {
		      PopOps(5);
		      ops.push(std::make_shared<Op>(val.value));
		    } else if (next->type == TokenType::Data) {
		    	values.push(std::make_shared<Variable>(-atoi(next->value.c_str())));
		    	++token;
		    } else if (next->type == TokenType::Paren_LEFT) {
	      	auto paren_right = find_pair_bracket(next + 1, end, TokenType::Paren_LEFT);
	      	if (paren_right == end)
					  throw std::logic_error("Wrong Syntax");
	        values.push(std::make_shared<Variable>(-Expression_evaluation(next + 1, paren_right)));
	        if ((token = paren_right) + 1 == end)
	          break;
	      } else {
	      	name_value[next->value] *= -1;
		    	values.push(std::make_shared<Variable>(name_value.at(next->value)));
		    	++token;
		    } 
	    } else if (val.value == "<" || val.value == "<=" || val.value == ">" || val.value == ">=") {
	      PopOps(4);
	      ops.push(std::make_shared<Op>(val.value));
	    } else if (val.value == "!=" || val.value == "==") {
	      PopOps(3);
	      ops.push(std::make_shared<Op>(val.value));
	    } else if (val.value == "&&") {
	      PopOps(2);
	      ops.push(std::make_shared<Op>(val.value));
	    } else if (val.value == "||") {
	      PopOps(1);
	      ops.push(std::make_shared<Op>(val.value));
	    } else {
	      if (val.type == TokenType::Data) {
	      	values.push(std::make_shared<Variable>(atoi(val.value.c_str())));
	      } else if (val.type == TokenType::Paren_LEFT) {
	      	auto paren_right = find_pair_bracket(token + 1, end, TokenType::Paren_LEFT);
	      	if (paren_right == end) 
					  throw std::logic_error("Wrong Syntax");
	        values.push(std::make_shared<Variable>(Expression_evaluation(token + 1, paren_right)));
	        if ((token = paren_right) + 1 == end)
	          break;
	      } else {
	        values.push(std::make_shared<Variable>(name_value.at(val.value)));
	      }
	    }
	
	    ++token;
	  }
	  
	  while (!ops.empty()) {
	    PopOps(0);
	  }
	  
	  auto node = values.top();
	  return node->Evaluate();
	}

	void Execution(const std::vector<Token>& tokens);  //running a ParaCl program based on parsed tokens
	
private:
	std::map<std::string, int> name_value;
};

bool operator==(const Token& t1, const Token& t2);  //comparing operator for tokens
