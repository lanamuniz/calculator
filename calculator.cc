//****************************************************************************//
//                                                                            //
//    calculator.cc - by Lana Muniz                                           //
//                                                                            //
//        Assumes input of the form: 34+5*12-311/8                            //
//        Supported operations: + - * /                                       //
//        Order of operations: / * - +                                        //
//        Limitations:                                                        //
//            -Parenthesis are not supported.                                 //
//            -Input like -4*2 or 2*-4 not supported.                         //
//            -Therefore, negative numbers are not supported.                 //
//            -Mathematical constants not supported.                          //
//            -Operations like ^, %, sqrt not supported.                      //
//                                                                            //
//    Compile and run:                                                        //
//                                                                            //
//        g++ -std=c++11 calculator.cc -o calculator                          //
//        ./calculator.exe                                                    //
//                                                                            //
//    Quit by entering q or Q.                                                //
//                                                                            //
//****************************************************************************//

#include <cstdlib>
#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <algorithm>

using std::string;
using std::cout;
using std::cin;
using std::endl;

//Order of operations is dictated by the REVERSE
//order they appear in the supported_operations vector.
const std::vector<string> supported_operations = {"+", "-", "*", "/"};
const std::set<char> supported_characters = {'0', '1', '2', '3', '4',
					     '5', '6', '7', '8', '9',
					     '.', ' ', '+', '-', '*', '/'};

bool illegal_calculation = false;

//****************************************************************************//
//                                                                            //
//    MathExpression class                                                    //
//                                                                            //
//****************************************************************************//
class MathExpression {
private :
  string s;
  void removeWhiteSpaces(string str) {
    str.erase(remove_if(str.begin(),
			      str.end(),
			      isspace),
		    str.end());
  }
public :
  MathExpression(string str) : s(str) {removeWhiteSpaces(s);}
  string get() {
    return s;
  }
  bool quitFlag() {
    if( (s.find("q") != std::string::npos) ||
	(s.find("Q") != std::string::npos) ) {
      return true;
    }
    return false;
  }
  bool isValid(const std::set<char> &allowed_chars) {
    if(s == "") {
      cout << endl << "Error: You did not enter an input." << endl;
      return false;
    }
    for(auto c : s) {
      if(allowed_chars.find(c) != allowed_chars.end()) { 
	continue;
      }
      else {
	cout << endl << "Error: " << c << " is not valid input." << endl;
	return false;
      }
    }
    return true;
  }
};

  
//****************************************************************************//
//                                                                            //
//    Functions for the main program                                          //
//                                                                            //
//****************************************************************************//
void printWelcomeMessage() {
  cout << endl << "This calculator supports the following operations:";
  for(auto op : supported_operations) {
    cout << " " << op;
  }
  cout << endl;
  cout << "To quit at any time, enter q or Q." << endl;
}

std::size_t nextOperatorPosition(string str) {
  //Find next operator to split on, in order +, -, *, /,
  //Returns std::string::npos if no operators are found.  
  std::size_t op_position;
  for(auto op : supported_operations) {
    op_position = str.find_first_of(op);
    if(op_position != std::string::npos) {
      break;
    }
  }
  return op_position;
}

double calcRecursive(MathExpression left, string my_op,
		     MathExpression right) {
  //Recursively split and evaluate the expression.

  //Check for input of the form: 8*+9, with
  //missing operands, and mark as illegal.
  if( (my_op != "") && (left.get()=="" || right.get()=="") ) {
      cout << endl << "Error: Missing operand." << endl;
      illegal_calculation = true;
      return 0;
  }    
  
  //Parse the first argument.
  std::size_t op_pos = nextOperatorPosition(left.get());
  double left_d;
  if(op_pos == std::string::npos) {
    //Base case - no operators
    left_d = atof(left.get().c_str());
  }
  else {
    left_d = calcRecursive(left.get().substr(0, op_pos),
			   left.get().substr(op_pos, 1),
			   left.get().substr(op_pos+1));
  }

  //Parse the second argument.
  op_pos = nextOperatorPosition(right.get());
  double right_d;
  if(op_pos == std::string::npos) {
    //Base case - no operators
    right_d = atof(right.get().c_str());
  }
  else {
    right_d = calcRecursive(right.get().substr(0, op_pos),
			    right.get().substr(op_pos, 1),
			    right.get().substr(op_pos+1));
  }

  //Do the calculation.
  if(my_op == "+") {
    return left_d + right_d;
  }
  if(my_op == "-") {
    return left_d - right_d;
  }
  if(my_op == "*") {
    return left_d * right_d;
  }
  if(my_op == "/") {
    if( right_d == 0 ) {
      cout << endl << "Error: Dividing by zero is not allowed." << endl;
      illegal_calculation = true;
      return 0;
    }
    else {
      return left_d / right_d;
    }
  }
}

double calcExpression(MathExpression expr) {
  //Wrapper function around calcRecursive.
  //Assumes the MathExpression has already been
  //checked for quit flag, unsupported characters,
  //and that white spaces have been removed.
  std::size_t op_pos = nextOperatorPosition(expr.get());
  if(op_pos == std::string::npos) { //no operators
    return atof(expr.get().c_str());
  }
  else {
    return calcRecursive(expr.get().substr(0, op_pos),
			 expr.get().substr(op_pos, 1),
			 expr.get().substr(op_pos+1));
  }
}


//****************************************************************************//
//                                                                            //
//    Main program                                                            //
//                                                                            //
//****************************************************************************//
int main() {
  
  printWelcomeMessage();
  
  bool quit_program = false;
  while(!quit_program) {
    //Prompt the user until they enter a valid expression
    bool prompt_user = true;
    string user_input = "";
    while(prompt_user) {
      cout << endl << "Enter an expression: ";
      getline(cin, user_input);
      MathExpression Input(user_input);
      if(Input.quitFlag()) {
	quit_program = true;
	break;
      }
      if(Input.isValid(supported_characters)) {
	prompt_user = false;
      }
      else {
	cout << "Invalid input. Please try again." << endl;
      }      
    }

    if(quit_program) {
      break;
    }
    else {
      illegal_calculation = false;
      MathExpression ExpressionToCalculate(user_input);
      double result = calcExpression(ExpressionToCalculate);
      if(illegal_calculation) {
	cout << "Illegal calculation. Please try again." << endl;
      }
      else {
	cout << user_input << "=" << result << endl;
      }
    }    
  }
  return 0;
}


