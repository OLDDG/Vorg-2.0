#include <iostream>
#include <sstream>
#include <windows.h> 
#include <string>
#include <cmath>
#include <map>
#include <iterator>

using namespace std;

class C2Ppn {
	char curc;                   //the current character
	string str_in, str_out;     //input & output strings          
	int iin;                     //the index of the input string

	char nextChar();           //get the next character from str_in
	bool isXorY(char c);
	int prior(char c);            //get the priority of the 
								  //character  
public:
	int calculate(string out, int x_in, int y_in);
	map<int, int> automat(string func, int x_in, int y_in, int iteration_count);
	void convert(string);        //convert to PPN                                          
	string get_str_out() const;   //get the output string
};

//stack
template <class T>
class Stack {
	T* pstack;
	int istack;
	int size;

	friend class C2Ppn;

	void push(T x) {
		if (istack < size) pstack[istack++] = x;
		else {
			MessageBox(NULL, (LPCWSTR)"Переполнение стека!", (LPCWSTR)"Error", MB_OK);
		}
	}
	T pop() {
		return (istack > 0) ? pstack[--istack] : EOS;
	}
	T top()const {
		return (istack > 0) ? pstack[istack - 1] : EOS;
	}
public:
	const char EOS;
	Stack() : EOS(0x1), size(50) {
		istack = 0;
		pstack = new T[size];
	}
	~Stack() { if (pstack) delete[] pstack; }
};


//if the character is a digit
inline bool C2Ppn::isXorY(char c) {
	return (c == 'x' || c == 'y' || c == 'X' || c == 'Y' || (c >= '0' && c <= '9'));
}

//the priopity of the operation
inline int C2Ppn::prior(char c) {
	switch (c) {
	case '(': return 1;
	case '|': case '&': case '#': return 2; //#-xor
	case '<': case '>': return 3;
	case '+': case '-': return 4;
	case '*': case '/': return 5;
	case '^': return 6;
	default: return 0;
	}
}

//get the next character from str_in
inline char C2Ppn::nextChar() {
	if (iin < str_in.length()) {
		return curc = str_in[iin++];
	}
	else return curc = '\0';
}

//get the output string
inline string C2Ppn::get_str_out()const {
	return str_out;
}

//convert to PPN
void C2Ppn::convert(string str) {
	int was_op = 0, np = 0;    //these flags show if there were 
							   //operations & brackets  
	iin = 0;                   //index of input string  
	Stack<char> op_stack;
	str_in = str;
	str_out = "";

	if ((!isXorY(str_in[0])) && str_in[0] != '(')
		throw (string)"Syntax error";

	while (nextChar() != '\0') {
		if (isXorY(curc)) {
			str_out += curc;
			was_op = 0;
			continue;
		}
		else str_out += ' ';

		switch (curc) {
		case '(':
			op_stack.push(curc);
			++np;
			was_op = 0;
			break;
		case '*': case '/': case '+': case '-': case '>':  case '<': case '#': case '&': case '|': case '^':
			if (iin == str_in.length())
				throw (string)"Syntax error";

			if (!was_op) {
				was_op = 1;
				while (prior(curc) <= prior(op_stack.top())) {
					str_out += op_stack.pop();
				}
				if (prior(curc) > prior(op_stack.top())) {
					op_stack.push(curc);
				}
				break;
			}
			else throw (string)"Syntax error";

		case ')':
			if (was_op)
				throw (string)"Syntax error";
			else
				while ((curc = op_stack.pop()) != '(' && np > 0) {
					str_out += curc;
				}
			--np;
			break;
		default: {
			throw (string)"Error: invalid symbol in the string";
		}
		}
	}
	while (op_stack.top() != op_stack.EOS)
		str_out += op_stack.pop();
	if (np)
		throw (string)"Error: wrong number of brackets";
}

int C2Ppn::calculate(string str_out, int x_in, int y_in) {
	Stack <int> stk;
	int numerator = 0;
	char check = str_out[numerator];
	int x = 0, y = 0;
	int result = 0;
	while (check != EOF && check != '\n' && check != '\0') {
		bool flag = true;
		string num = "";
		while (check == ' ' || check == '\t') {
			check = str_out[++numerator];
		}
		if (check == 'x' || check == 'X') stk.push(x_in);
		else if (check == 'y' || check == 'Y') stk.push(y_in);
		else if (check <= '9' && check >= '0') {
			while (check <= '9' && check >= '0') {
				num += check;
				check = str_out[++numerator];
				flag = false;
			}
			stk.push(stoi(num));
		}
		else {
			y = stk.top(); stk.pop();
			x = stk.top(); stk.pop();
			switch (check) {
				case '+':
					stk.push(x + y); break;
				case '-':
					stk.push(x - y); break;
				case '*':
					stk.push(x * y); break;
				case '/':
					stk.push(x / y); break;
				case '|':
					stk.push(x | y); break;
				case '#': //xor
					stk.push(x ^ y); break;
				case '&':
					stk.push(x & y); break;
				case '<':
					stk.push(x << y); break;
				case '>':
					stk.push(x >> y); break;
				case '^':
					stk.push((int)pow(x, y)); break;
			}
		}
		if (flag) numerator++;
		check = str_out[numerator];
	}
	return stk.top();
}

string removeSpaces(string input)
{
	input.erase(std::remove(input.begin(), input.end(), ' '), input.end());
	return input;
}

int main()
{
	try {
		string str_in;
		cout << '\n' << "Enter a function F(x, y) = " << endl;
		getline(cin, str_in);                //get the input string
		int x, y, iteration_count;
		cout << '\n' << "Enter the first argument x:" << endl;
		cin >> x;
		cout << '\n' << "Enter the second argument y:" << endl;
		cin >> y;
		cout << '\n' << "Enter the number of iterations N:" << endl;
		cin >> iteration_count;
		if (str_in == "\0") return 0;
		C2Ppn ppn;
		ppn.convert(removeSpaces(str_in));
		string function = (string)ppn.get_str_out();
		map<int, int> iter_value;
		for (int i = 0; i < iteration_count; i++) {
			int calc = ppn.calculate(function, x, y);
			iter_value[i] = calc;
			cout << '\n' << "Iteration number:" << i << "      Value:" << calc << endl;
			x = y;
			y = calc;
		}
	}
	catch (LPCSTR exc) {
		cout << (LPCSTR)exc << '\n' << endl;
	}
return 0;
}