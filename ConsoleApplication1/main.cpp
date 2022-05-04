#include <iostream>
#include <windows.h> 
#include <string>

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
	int calculate(string in, string out);
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
	return (c >= '0' && c <= '9'); //(c == 'x' || c == 'y' || c == 'X' || c == 'Y');
}

//the priopity of the operation
inline int C2Ppn::prior(char c) {
	switch (c) {
	case '(': return 1;
	case '+': case '-': return 2;
	case '*': case '/': return 3;
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
		case '*': case '/': case '+': case '-':
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

int C2Ppn::calculate(string str_in, string str_out) {
	Stack<int> val_stack; //стек
	int n1 = 0, n2 = 0, res;

	for (int i = 0; i < str_in.length(); ++i) {
		if (C2Ppn::isXorY(str_out[i])) {
			val_stack.push(std::atoi(&str_out[i]));
		}
		else {
			if (str_out[i] == ' ') continue;
			n2 = val_stack.top(); val_stack.pop();
			n1 = val_stack.top(); val_stack.pop();
			switch (str_out[i]) {
			case '+': res = n1 + n2; break;
			case '-': res = n1 - n2; break;
			case '*': res = n1 * n2; break;
			case '/': res = n1 / n2; break;
			default: cout << "Ошибка !\n";
			}
			val_stack.push(res);
		}
	}
	return val_stack.pop();
}

int main()
{
	try {
		char buf[50];
		string str_in;
		cout << "Introduce the expression (use only numbers," << endl;
		cout << "arithmetical operations and brackets," << endl;
		cout << "introduce 0 to exit the program):" << endl;
		cin >> buf;                //get the input string
		if (buf[0] == '\0') return 0;
		str_in = buf;
		C2Ppn ppn;
		ppn.convert(str_in);
		cout << '\n' << ppn.calculate(str_in, (string)ppn.get_str_out()) << '\n' << endl;
	}
	catch (LPCSTR exc) {
		cout << (LPCSTR)exc << '\n' << endl;
	}
return 0;
}