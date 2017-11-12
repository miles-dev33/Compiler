// Brainich, Adam K; adambrainich@csu.fullerton.edu
// McCloskey, Miles; miles.mccloskey@csu.fullerton.edu
// Nghiem, Alec; Kou.alec@gmail.com
// CPSC 323
// Final Project

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

bool isReserved(string word) {
	string reserved[] = { "PROGRAM", "INTEGER", "PRINT", "BEGIN", "END." };
	bool res = false;

	for (int i = 0; i < 5; i++) {
		if (word == reserved[i])
			res = true;
	}

	return res;
}

bool isVariable(string word) {
	bool var = true;

	for (int i = 0; i < word.length(); i++) {
		if (word[i] != 'P' && word[i] != 'Q' && word[i] != 'R' && word[i] != 'S' && !isdigit(word[i]))
			var = false;
	}

	return var;
}

bool isSpecial(char ch) {
	char special[9] = { '=', ',', ':', ';', '(', ')', '+', '-', '*' };
	bool spec = false;

	for (int i = 0; i < 9; i++) {
		if (ch == special[i])
			spec = true;
	}

	return spec;
}

bool isTerminal(string test) {
	string terminals[30] =
	{ "PROGRAM","BEGIN","END.","INTEGER","PRINT",
		"$",";",":",",","(",")","+","-","*","/","=",
		"P","Q","R","S","0",
		"1","2","3","4","5","6","7","8","9" };
	for (int i = 0; i < 30; i++) {
		if (test == terminals[i])
			return true;
	}
	return false;
}

int rows(string non_terminal) {
	string non_terminals[22] = { 
		"prog","identifier","more-id-digit","dec-list","dec","dec-prime",
		"type","stat-list","stat-prime","stat","print","assign","expr","expr-prime",
		"term","term-prime","factor","number","sign","more-digit","digit","id" };
	for (int i = 0; i < 22; i++) {
		if (non_terminal == non_terminals[i])
			return i;
	}
}

int cols(string terminal) {
	string terminals[30] = 
	{ "PROGRAM","BEGIN","END.","INTEGER","PRINT",
		"$",";",":",",","(",")","+","-","*","/","=",
		"P","Q","R","S","0",
		"1","2","3","4","5","6","7","8","9" };
	for (int i = 0; i < 30; i++) {
		if (terminal == terminals[i])
			return i;
	}
}

int parse(int non_terminal, int terminal) {
	int table[22][30] = {
		{ 2,50,50,50,50,	50,50,50,50,51,52,		50,50,50,50,50,		50,50,50,50,	50,50,50,50,50,50,50,50,50,50 },
		{ 44,44,44,44,44,	44,44,44,44,51,52,		44,44,44,44,44,		18,18,18,18,	44,44,44,44,44,44,44,44,44,44 },
		{ 47,45,47,45,47,	47,1,47,1,51,1,			1,1,1,1,1,			18,18,18,18,	19,19,19,19,19,19,19,19,19,19 },
		{ 42,42,42,3,42,	42,42,42,42,51,52,		42,42,42,42,42,		42,42,42,42,	42,42,42,42,42,42,42,42,42,42 },
		{ 44,44,44,44,44,	44,44,44,44,51,52,		44,44,44,44,44,		20,20,20,20,	44,44,44,44,44,44,44,44,44,44 },
		{ 47,47,47,47,47,	47,1,47,9,51,52,		47,47,47,47,47,		47,47,47,47,	47,47,47,47,47,47,47,47,47,47 },	 // dec-prime  
		{ 47,47,47,4,47,	47,47,47,47,51,52,		47,47,47,47,47,		47,47,47,47,	47,47,47,47,47,47,47,47,47,47 },
		{ 47,47,47,47,5,	47,47,47,47,51,52,		47,47,47,47,47,		5,5,5,5,		47,47,47,47,47,47,47,47,47,47 },
		{ 47,47,1,47,6,		47,47,47,47,51,52,		47,47,47,47,47,		6,6,6,6,		47,47,47,47,47,47,47,47,47,47 },	// stat-prime
		{ 47,47,47,47,7,	47,47,47,47,51,52,		47,47,47,47,47,		21,21,21,21,	47,47,47,47,47,47,47,47,47,47 },
		{ 47,47,47,47,8,	47,47,47,47,51,52,		47,47,47,47,47,		47,47,47,47,	47,47,47,47,47,47,47,47,47,47 },
		{ 47,47,47,47,47,	47,47,47,47,51,52,		47,47,47,47,47,		22,22,22,22,	47,47,47,47,47,47,47,47,47,47 },
		{ 47,47,47,47,47,	47,47,47,47,10,52,		10,10,46,46,46,		10,10,10,10,	10,10,10,10,10,10,10,10,10,10 },
		{ 47,47,47,47,47,	47,1,47,47,51,1,		23,24,47,47,47,		47,47,47,47,	47,47,47,47,47,47,47,47,47,47 },	// expr-prime
		{ 47,47,47,47,47,	47,47,47,47,11,52,		11,11,47,47,47,		11,11,11,11,	11,11,11,11,11,11,11,11,11,11 },
		{ 47,47,47,47,45,	47,1,47,47,51,1,		1,1,41,13,46,		46,46,46,46,	46,46,46,46,46,46,46,46,46,46 },	// term-prime
		{ 47,47,47,47,47,	47,47,47,47,12,52,		14,14,46,46,47,		25,25,25,25,	14,14,14,14,14,14,14,14,14,14 },
		{ 47,47,47,47,47,	47,47,47,47,51,52,		15,15,47,47,47,		47,47,47,47,	15,15,15,15,15,15,15,15,15,15 },
		{ 47,47,47,47,47,	47,47,47,47,51,52,		16,17,47,47,47,		47,47,47,47,	1,1,1,1,1,1,1,1,1,1 },
		{ 47,47,47,47,45,	47,1,47,47,51,1,		1,1,1,1,47,			45,45,45,45,	26,26,26,26,26,26,26,26,26,26 },
		{ 47,47,47,47,47,	47,47,47,47,51,52,		46,46,47,47,47,		47,47,47,47,	31,32,33,34,35,36,37,38,39,40 },
		{ 47,47,47,47,47,	47,47,47,47,51,52,		47,47,47,47,47,		27,28,29,30,	47,47,47,47,47,47,47,47,47,47 } };
	return table[non_terminal][terminal];
}

void errorCheck(string popped) {
	if (popped == "PROGRAM")
		cout << "PROGRAM is expected\n";
	else if (popped == "BEGIN")
		cout << "BEGIN is expected\n";
	else if (popped == "END.")
		cout << "END. is expected\n";
	else if (popped == "INTEGER")
		cout << "INTEGER is expected\n";
	else if (popped == ";")
		cout << "; is missing\n";
	else if (popped == ":")
		cout << ": is missing\n";
	else if (popped == ",")
		cout << ", is missing\n";
	else if (popped == "(")
		cout << "Left parenthesis is missing\n";
	else if (popped == ")")
		cout << "Right parenthesis is missing\n";
	else if (popped == "PRINT")
		cout << "PRINT is expected\n";
	else
		cout << "Illegal expression\n";
}

int main() {
	string line, sub, popped, test;
	ifstream input;
	ofstream output;
	int len, place;
	bool print, comment = false, fail = false;
	stack <string> s;

	// Part 1

	input.open("finalv1.txt");
	output.open("finalv2.txt");

	while (getline(input, line)) {
		len = line.length();
		print = false;	// reset

		for (int i = 0; i < len; i++) {
			if (comment == false) {
				if (isalpha(line[i])) {
					print = true;
					place = i;
					sub = "";	// reset

					while (!isReserved(sub) && !isSpecial(line[i]) && line[i] != ' ' && line[i] != '\t' && i < len) {
						sub = line.substr(place, i - place);
						i++;
					}

					sub = line.substr(place, i - place);
					output << sub;
					if (isReserved(sub) || isVariable(sub))
						output << " ";
					i--;
				}
				else if (line[i] == '/' && line[i + 1] == '/') {
					comment = true;
					i++;
				}
				else if (isdigit(line[i]) && isdigit(line[i + 1])) {
					print = true;
					output << line[i];
				}
				else if (line[i] == ' ') {}
					// do nothing
				else {
					print = true;
					output << line[i] << " ";
				}
			}
			else if (line[i] == '/' && line[i + 1] == '/') {
				comment = false;
				i++;
			}
		}

		if (print)
			output << '\n';
	}

	input.close();
	output.close();


	// Part 2

	input.open("finalv2.txt");
	s.push("prog");

	while (getline(input, line) && fail == false) {
		len = line.length();
		for (int i = 0; i < len; i++) {
			if (line[i] != ' ') {
				place = i;
				sub = "";	// reset
				while (line[i] != ' ' && i < len)
					i++;
				sub = line.substr(place, i - place);

				if (isReserved(sub))
					test = sub;
				else {
					i = place;
					test = line[i];
				}

				if ((i < len - 1 || test.length() == len - 1) && (!s.empty())) {
					popped = s.top();
					s.pop();
				}

				while (popped != test && fail == false) {
					if ((isTerminal(popped) && test != popped) || (!isTerminal(test))) {
						i = len;		// skip to end of line
						fail = true;	// end while loop
						errorCheck(popped);
					}

					if (fail == false) {
						switch (parse(rows(popped), cols(test))) {
						case 0: fail = true;	// failure
								cout << "Illegal expression\n"; break;
						case 1: break;			// lambda
						case 2: s.push("END."); s.push("stat-list"); s.push("BEGIN"); s.push("dec-list"); 
								s.push(";"); s.push("identifier"); s.push("PROGRAM"); break;
						case 3: s.push(";"); s.push("dec"); s.push(":"); s.push("type"); break;
						case 4: s.push("INTEGER"); break;
						case 5: s.push("stat-prime"); s.push("stat"); break;
						case 6: s.push("stat-list"); break;
						case 7: s.push("print"); break;
						case 8: s.push(";"); s.push(")"); s.push("identifier"); s.push("("); s.push("PRINT"); break;
						case 9: s.push("dec"); s.push(","); break;
						case 10: s.push("expr-prime"); s.push("term"); break;
						case 11: s.push("term-prime"); s.push("factor"); break;
						case 12: s.push(")"); s.push("expr"); s.push("("); break;
						case 13: s.push("term-prime"); s.push("factor"); s.push("/"); break;
						case 14: s.push("number"); break;
						case 15: s.push("more-digit"); s.push("digit"); s.push("sign"); break;
						case 16: s.push("+"); break;
						case 17: s.push("-"); break;
						case 18: s.push("more-id-digit"); s.push("id"); break;
						case 19: s.push("more-id-digit"); s.push("digit"); break;
						case 20: s.push("dec-prime"); s.push("identifier"); break;
						case 21: s.push("assign"); break;
						case 22: s.push(";"); s.push("expr"); s.push("="); s.push("identifier"); break;
						case 23: s.push("expr-prime"); s.push("term"); s.push("+"); break;
						case 24: s.push("expr-prime"); s.push("term"); s.push("-"); break;
						case 25: s.push("identifier"); break;
						case 26: s.push("more-digit"); s.push("digit"); break;
						case 27: s.push("P"); break;
						case 28: s.push("Q"); break;
						case 29: s.push("R"); break;
						case 30: s.push("S"); break;
						case 31: s.push("0"); break;
						case 32: s.push("1"); break;
						case 33: s.push("2"); break;
						case 34: s.push("3"); break;
						case 35: s.push("4"); break;
						case 36: s.push("5"); break;
						case 37: s.push("6"); break;
						case 38: s.push("7"); break;
						case 39: s.push("8"); break;
						case 40: s.push("9"); break;
						case 41: s.push("term-prime"); s.push("factor"); s.push("*"); break;
						case 50: fail = true; cout << "PROGRAM is expected\n"; break;
						case 42: fail = true; cout << "INTEGER is expected\n"; break;
						case 43: fail = true; cout << "Unacceptable variable name\n"; break;
						case 44: fail = true; cout << "Variable is expected\n"; break;
						case 45: fail = true; cout << "; is expected\n"; break;
						case 46:
						case 47: 
						case 48: fail = true; cout << "Illegal expression\n"; break;
						case 51: fail = true; cout << "( is expected\n"; break;
						case 52: fail = true; cout << ") is expected\n"; break;
						}
					}
					if (fail == false) {
						popped = s.top();
						s.pop();
					}
				}
			}
		}
	}

	if (!s.empty())
		fail = true;


	// Part 3

	if (fail == false) {
		string line, substr;
		ifstream inFile;

		inFile.open("finalv2.txt");

		while (!inFile.eof())
		{
			getline(inFile, line);
			if (line.find("PROGRAM") != string::npos) {
				cout << "#include <iostream>" << endl;
				cout << "using namespace std" << endl;
			}
			else if (line.find("INTEGER") != string::npos)
				substr = line.substr(10, line.length());
			else if (line.find("BEGIN") != string::npos) {
				cout << "int main()\n{" << endl;
				cout << "\tint ";
				cout << substr;
				cout << endl;
			}
			else if (line.find("=") != string::npos)
				cout << "\t" << line << endl;
			else if (line.find("PRINT") != string::npos) {
				int temp = line.find(")");
				cout << "\tcout << " << line.substr(8, temp - 8) << "<< endl;" << endl;
			}
			else if (line.find("END.") != string::npos) {
				cout << "\treturn 0;\n";
				cout << "}";
			}
		}

		cout << endl;
	}
	

	system("PAUSE");
	return 0;
}