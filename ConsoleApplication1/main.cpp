#include <iostream>
#include <sstream>
#include <windows.h> 
#include <string>
#include <cmath>
#include <map>
#include <iterator>
#include <vector> 
#include <fstream>
#include <limits>

using namespace std;

class C2Ppn {
	char curc;                   //the current character
	string str_in, str_out;     //input & output strings          
	int iin;                     //the index of the input string

	char nextChar();           //get the next character from str_in
	bool isXorY(char c);
	int prior(char c);            //get the priority of the character  

public:
	long long calculate(string out, long long x_in, long long y_in);
	//map<int, int> automat(string func, int x_in, int y_in, int iteration_count);
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

long long C2Ppn::calculate(string str_out, long long x_in, long long y_in) {
	Stack <long long> stk;
	long long numerator = 0;
	char check = str_out[numerator];
	long long x = 0, y = 0;
	long long result = 0;
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
					stk.push((long long)(x / y)); break;
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
					stk.push((long long)(pow((double)x, (double)y))); break;
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

string to_2adic_Reverse(long long x) {
	string buffer;
	buffer.reserve(numeric_limits<unsigned long long>::digits);
	bool neg = false;
	if (x < 0) {
		x = -x;
		neg = true;
	}
	do
	{
		buffer += char('0' + x % 2);
		x /= 2;
	} while (x > 0);
	string s = string(buffer.crbegin(), buffer.crend());
	size_t pos = 0;
	if (neg) {
		for (long long i = 0; i < s.length(); i++) {
			s[i] = (char)(1 - (int)(s[i] - '0') + '0');
		}
		for (int i = s.length() - 1; i > 0; i--) {
			if (s[i] == '0') {
				s[i] = '1'; break;
			}
			else s[i] = '0';
		}
	}
	s.insert(pos, "0"); pos++;
	s.insert(pos, ".");
	return s;//stoll(buffer, NULL, 10);
}


string to_2adic_Mon(long long x) {
	string buffer; 
	buffer.reserve(numeric_limits<unsigned long long>::digits);
	bool neg = false;
	if (x < 0) {
		x = -x;
		neg = true;
	}
	do
	{
		buffer += char('0' + x % 2);
		x /= 2;
	} while (x > 0);
	size_t pos = 0;
	if (neg) {
		for (long long i = 0; i < buffer.length(); i++) {
			buffer[i] = (char)(1 - (int)(buffer[i] - '0') + '0');
		}
		for (int i = buffer.length() - 1; i > 0; i--) {
			if (buffer[i] == '0') {
				buffer[i] = '1'; break;
			}
			else buffer[i] = '0';
		}
	}
	buffer.insert(pos, "0"); pos++;
	buffer.insert(pos, ".");
	return buffer;
}


int main()
{
	try {
		string str_in, rubish;
		int graph_type;
		int mapp_style;
		fstream FILE;
		FILE.open("Values.txt", fstream::out);
		cout << '\n' << "Enter graph type: (0)graph or (1)subsequence" << '\n' << endl;
		cin >> graph_type;
		getline(cin, rubish);
		if (graph_type > 1) return 0;
		if (!graph_type) {
			vector <int> x, y;
			cout << '\n' << "Enter in-value type: (0)range or (1)by yourself" << '\n' << endl;
			int in_value_type;
			cin >> in_value_type;
			getline(cin, rubish);
			if (in_value_type > 1) return 0;
			int N; // amount of (x, y)
			if (in_value_type) {
				vector <long long> x, y;
				cout << '\n' << "Enter amount of argument pairs (x, y) N = " << endl;
				cin >> N;
				getline(cin, rubish);
				cout << '\n' << "Enter your " << N << " first arguments x" << endl;
				long long x_i;
				cin >> x_i;
				getline(cin, rubish);
				for (int i = 0; i < N; i++) x.push_back(x_i);
				cout << '\n' << "Enter your " << N << " first arguments y" << endl;
				long long y_i;
				cin >> y_i;
				getline(cin, rubish);
				for (int i = 0; i < N; i++) y.push_back(y_i);
			}
			cout << '\n' << "Enter your function F(x, y) = " << endl; ///TODO
			getline(cin, str_in);
			if (str_in == "\0") return 0;
			cout << '\n' << "Choose your mapping (1)Mona or (2)Reverse" << endl;
			cin >> mapp_style;
			getline(cin, rubish);
			C2Ppn ppn;
			ppn.convert(removeSpaces(str_in));
			string function = (string)ppn.get_str_out();
			map<long long, string> iter_value;
			long long numerator = 0;
			if (x.size() == 0 || y.size() == 0) {				
				for (long long i = 0; i < (long long)pow(2, 10); i++) {
					for (long long j = 0; j < (long long)pow(2, 10); j++) { ///TODO change all to long long values and think about double
						long long calc = ppn.calculate(function, i, j);
						if (mapp_style == 1) {
							iter_value[numerator] = to_2adic_Mon(i);numerator++;
							iter_value[numerator] = to_2adic_Mon(j);numerator++;
							iter_value[numerator] = to_2adic_Mon(calc);numerator++;
						}
						if (mapp_style == 2) {
							iter_value[numerator] = to_2adic_Reverse(i); numerator++;
							iter_value[numerator] = to_2adic_Reverse(j); numerator++;
							iter_value[numerator] = to_2adic_Reverse(calc); numerator++;
						}
					}
				}
			}
			else {
				long long numerator = 0;
				for (long long i = 0; i < N; i++) {
					long long calc = ppn.calculate(function, x[i], y[i]);
					if (mapp_style == 1) {
						iter_value[numerator] = to_2adic_Mon(x[i]); numerator++;
						iter_value[numerator] = to_2adic_Mon(y[i]); numerator++;
						iter_value[numerator] = to_2adic_Mon(calc); numerator++;
					}
					if (mapp_style == 2) {
						iter_value[numerator] = to_2adic_Reverse(x[i]); numerator++;
						iter_value[numerator] = to_2adic_Reverse(y[i]); numerator++;
						iter_value[numerator] = to_2adic_Reverse(calc); numerator++;
					}
				}
			}
			numerator = 0;
			for (long long i = 0; i < iter_value.size() / 3; i++) {
				FILE << iter_value[numerator] << "   "; numerator++;
				FILE << iter_value[numerator] << "   "; numerator++;
				FILE << iter_value[numerator] << "   " << endl; numerator++;
			}
			FILE.close();
			///TODO TEST THIS SHIT + write to file + python printing
		}
		else {
			long long x, y;
			int iteration_count;
			cout << '\n' << "Enter the number of iterations N:" << '\n' << endl;
			cin >> iteration_count;
			getline(cin, rubish);
			cout << '\n' << "Enter initial value (x, y)" << '\n' << endl;
			cout << '\n' << "Enter the first argument x:" << '\n' << endl;
			cin >> x;
			getline(cin, rubish);
			cout << '\n' << "Enter the second argument y:" << '\n' << endl;
			cin >> y;
			getline(cin, rubish);
			cout << '\n' << "Enter a function without spaces F(x, y) = " << '\n' << endl;
			getline(cin, str_in);
			cout << '\n' << "Choose your mapping (1)Mona or (2)Reverse" << endl;
			cin >> mapp_style;
			getline(cin, rubish);
			if (str_in == "\0") return 0;
			C2Ppn ppn;
			ppn.convert(removeSpaces(str_in));
			string function = (string)ppn.get_str_out();
			map<long long, string> iter_value;
			long long numerator = 0;
			for (int i = 0; i < iteration_count; i++) {
				int calc = ppn.calculate(function, x, y);
				if (mapp_style == 1) {
					iter_value[numerator] = to_2adic_Mon(x); numerator++;
					iter_value[numerator] = to_2adic_Mon(y); numerator++;
					iter_value[numerator] = to_2adic_Mon(calc); numerator++;
				}
				if (mapp_style == 2) {
					iter_value[numerator] = to_2adic_Reverse(x); numerator++;
					iter_value[numerator] = to_2adic_Reverse(y); numerator++;
					iter_value[numerator] = to_2adic_Reverse(calc); numerator++;
				}
				x = y;
				y = calc;
			}
			numerator = 0;
			for (long long i = 0; i < iter_value.size() / 3; i++) {
				FILE << iter_value[numerator] << "   "; numerator++;
				FILE << iter_value[numerator] << "   "; numerator++;
				FILE << iter_value[numerator] << "   " << endl; numerator++;
			}
			FILE.close();
		}
	}
	catch (LPCSTR exc) {
		cout << (LPCSTR)exc << '\n' << endl;
	}
return 0;
}