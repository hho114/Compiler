#pragma once
#ifndef LEXICAL_H
#define LEXICAL_H

#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <queue>
#include "Token.h"
#include "SyntaxAnalyzer.h"
using namespace std;





//accept 1,2
//input letter, digit, and unknown
//Regular Expression = l|l(l|d)*l
// Note: "->" mean go to next state 
const int idFsmTable [5][3] = { {1,4,4}, //state 0, starting state, accepts only letter as input
								{2,3,4 }, //state 1, letter->2,digit->3, unknown->4
								{2,3,4},	//state 2, letter->2,digit->3, unknown->4
								{2,3,4},	//state 3, letter->2,digit->3, unknown->4
								{4,4,4} };   //state 4, letter->4,digit->4, unknown->4

enum mColIdFsmTable {Letter,Digit,Unknown}; //Column id for FSM Identifier

//accept 1,3
// Regular Expression = d.d
//input digit, '.', and unknown
// Note: "->" mean go to next state 
const int intRealFsmTable[5][3] = { {1,4,4}, //state 0, starting state, accepts only digit as input
									{1,2,4}, //state 1, ditgit->1,decimal->2, unknown->4
									{3,4,4}, //state 2, ditgit->3,decimal->4, unknown->4
									{3,4,4}, //state 3, ditgit->3,decimal->4, unknown->4
									{4,4,4} }; //state 4, ditgit->4,decimal->4, unknown->4

//Column id for FSM Digits
enum mColIntRealFsmTable{DigitReal,Decimal}; //Note Unknown already been created in mColIdFsmTable

//Array of string keywords
const string keywords[14] = { "int","real","boolean","if","else","ifend","while",
							 "return","get","put","whileend","function","true","false" };
//Array of string operator
const string operators[11] = { "=","+","-","*","/","=>","=<","^=","==",">","<" };
//Arrat of strig seperator
const string seperators[9] = { "$$", ",", ":", ";", "|", "(", ")","{", "}"};

class Lexical {


public:
	Lexical() { mStillInComment = false; mLineNumber = 0; }//Constructor
	//Lexical operator= (Lexical const & object);
	
	bool isOperator(string token);//Check whether string is operator
	bool isSeparator(string token); //Check whether string is separator
	bool isKeyword(string token);	//check whether string is keyword
	bool isReal(string token);		//check whether string is real
	bool isLetterFirst(string token);	//check whether first character of string is letter 

	int nextStateIdFsm(char input) const;//check input to determine next state of Identifier FSM
	int nextStateIntRealFsm(char input) const; //check input to determine next state of digit FSM

	/*Lexer function to scanner one string line
	  Determine tokens by charactor
	  Put into a new member token
	  Push that member token into queue list*/
	void lexer(string);

	//Scanner to decide whether comment have done
	void commentScanner(string);

	//Print list of token and lexeme in console and out put into a new file name Output
		
	queue<Token> getListToken();
	~Lexical();
private:

	bool mStillInComment; 
	int mLineNumber;
	queue <Token> mListToken; //queue list Token
};



#endif #pragma once
