#pragma once
#ifndef SYNTAX_ANALYZER_H
#define SYSTAX_ANALYZER_H

#include"Lexical.h"
#include <queue>
#include <string>
#include <ostream>
#include<fstream>

using namespace std;
/*List qualifiers and relops*/
const string qualifierList[5] = { "real","boolean","int" };
const string relopList[6] = { "==","^=",">","<","=>","=<" };




class SyntaxAnalyzer
{
	
	
public:
	/*Default contructor for SyntaxAnalyzer */
	SyntaxAnalyzer();
	/*Overload constructor for SystaxAnalyzer 
	parameter: list token*/
	SyntaxAnalyzer(queue <Token> list);
	/*Destructor for SyntaxAnalyzer*/
	~SyntaxAnalyzer();
	/*Error message printer
	parameter: string of error, string of rule name*/
	void errorMessage(string,string);
	/**/
	void nextToken();
	//friend ostream & operator<<(ostream & out, SyntaxAnalyzer &);
	void printListSyntax();

	/*Syntax rule functions*/
	void Rat18F();
	void OptFunctionDefinition();
	void FunctionDefinition();
	void Function();
	void OptParameterList();
	void ParameterList();
	void Parameter();
	void Qualifier();
	void Body();
	void OptDeclarationList();
	void DeclarationList();
	void Declaration();
	void IDs();
	void StatementList();
	void Statement();
	void Compound();
	void Assign();
	void If();
	void Return();
	void Print();
	void Scan();
	void While();
	void Condition();
	void Relop();
	void Expression();
	void ExpressionPrime();
	void Term();
	void TermPrime();
	void Factor();
	void Primary();
	void Empty();
	


private:
	
	string mProductionRules;//contain current production rules
	Token mCurrentTokenObject;// contain current token object of list
	string mCurrentLexeme;//contain current lexeme
	string mCurrentToken;//contain current token
	queue <Token> mListToken;//this list is used to get from the list of lexical analyzer 
	queue <Token> mListTokenSyntax;// final list after syntax analyzer

};



#endif // !SYNTAX_ANALYZER_H
