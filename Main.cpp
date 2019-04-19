#include<iostream>
#include<string>
#include<fstream>
#include"Lexical.h"
#include"Token.h"
#include"SyntaxAnalyzer.h"

using namespace std;


void optionOne();
void optionTwo();
void testCase(string fileInput);

int main()
{
	cout << "****************************************\n"
		<< "WElCOME TO CS 323 COMPILER APPLICATION\n"
		<< "****************************************\n";
	int input = 0;
	do {
			cout<< "Menu\n"
			<< "1. Write your own test case.\n"
			<< "2. Run 3 test cases\n"
			<< "3. Exit Application\n\n"
			<< "Please enter number option: ";
		cin >> input;
		cin.clear();
		cin.ignore(1000, '\n');

		while (input > 3 || input < 1)
		{
			cout << "\nPlease reenter number option: ";
			cin >> input;
			cin.clear();
			cin.ignore(1000, '\n');
		}

		switch (input)
		{
			case 1:
				optionOne();
				break;
			case 2:
				optionTwo();
				break;
			case 3:
				cout << "Thank you for using program, have a great day! :)" << endl;
				break;
			default:
				break;
		}
		cout << endl;
	} while (input != 3);

	cout << endl;
	system("pause");
	return 0;
}



void optionOne()
{
	
	Lexical lexi;
	string inputLine;
	cout << "Please write underneath one line test case for compiler." << endl;
	getline(cin, inputLine);
	lexi.lexer(inputLine);
	cout << "Here your output for token and lexeme:\n";
	SyntaxAnalyzer syntax(lexi.getListToken());
	syntax.printListSyntax();
	cout << "\nNOTE: The compiler also write out to Output.txt,\n"
		<< "check it out at /Project Compiler CS 323/ Project Compiler/ Output.txt" << endl;
	cout << endl;
}


void optionTwo()
{

	int level = 0;
	cout << "Which test case would you like to see?\n"
		<< "1. Level 1\n"
		<< "2. Level 2\n"
		<< "3. Level 3\n"
		<< "Please enter number option: ";
	cin >> level;
	cin.clear();
	cin.ignore(1000, '\n');

	while (level > 3 || level < 1)
	{
		cout<< "\nPlease reenter number option: ";
		cin >> level;
		cin.clear();
		cin.ignore(1000, '\n');
	}
	
	switch (level)
	{
	case 1:
		testCase("test1.txt");
		break;

	case 2:
		testCase("test2.txt");
		break;

	case 3:
		testCase("test3.txt");
		break;

	default:
		break;
	}
	if(level == 1 || level == 2 || level == 3)
	cout << "\nNOTE: The compiler also write out to Output.txt,\n"
		<< "check it out at /Project Compiler CS 323/ Project Compiler/ Output.txt" << endl;
}


void testCase(string fileInput)
{
	string currentLine;
	Lexical l;
	fstream inFile;

	inFile.open(fileInput, ios::in);
	cout << endl;
	while (getline(inFile, currentLine))
	{
		cout << currentLine << endl;
		l.lexer(currentLine);
	}
	cout << endl;
	SyntaxAnalyzer syntax(l.getListToken());
	syntax.printListSyntax();
	inFile.close();
}

