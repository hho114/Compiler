#include"Token.h"
#include<iostream>
#include<string>

/*Decleration of token functions and constructor*/

Token::Token(string toke, string lexeme, int lineNum, int id)
{
	setToken(toke);
	setLexeme(lexeme);
	setLineNum(lineNum);
	setFsmId(id);
}

Token::Token(string toke, string lexeme, string rule, int lineNum, int id)
{
	Token(toke, lexeme, lineNum, id);
	setProductionRule(rule);
}

Token & Token::operator=(Token const & object)
{
	mToken = object.mToken;
	mLexeme = object.mLexeme;
	mNumState = object.mNumState;
	mLineNum = object.mLineNum;
	mProductionRule = object.mProductionRule;

	return *this;
}

void Token::setToken(string toke) { mToken = toke; }
void Token::setLexeme(string lexe) { mLexeme = lexe; }
void Token::setLineNum(int num) { mLineNum = num; }
void Token::setFsmId(int id) { mNumState = id; }

void Token::setProductionRule(string rule)
{
	mProductionRule = rule;
}

int Token::getFsmId()const { return mNumState; }
int Token::getLineNum()const { return mLineNum; }
string Token::getToken()const { return mToken; }
string Token::getLexeme()const { return mLexeme; }

string Token::getProductionRule() const
{
	return mProductionRule;
}

Token::~Token(){}

ostream & operator<<(ostream & output, const Token toke)
{
	output  <<"Token: " << left << setw(15) << toke.getToken()  << "Lexeme: "<<toke.getLexeme() << endl
		<< toke.getProductionRule()<<endl;

	return output;
}