#pragma once
#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <iomanip>


using namespace std;
/* Token class is store the token values for lexical analyzer */

class Token
{
	friend ostream & operator<<(ostream &, const Token);

public:
	Token() { Token("na", "na","", 0, 0); } //default constructor
	Token(string toke, string lexeme, int lineNum, int id); // overload constructor
	Token(string toke, string lexeme, string rule, int lineNum, int id);
	//Overload operator
	Token & operator = (Token const & object);

	//Retriever Token values
	void setToken(string);
	void setLexeme(string);
	void setLineNum(int);
	void setFsmId(int);
	void setProductionRule(string);

	//accessor Token values
	int getFsmId()const;
	int getLineNum()const;
	string getToken()const;
	string getLexeme()const;
	string getProductionRule()const;

	~Token(); //Destructor

	
private:
	string mToken; //token name
	string mLexeme; // token lexeme
	int mLineNum; //line
	int mNumState;
	string mProductionRule;

};


#endif 