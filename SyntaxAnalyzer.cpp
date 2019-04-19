#include "SyntaxAnalyzer.h"


/*Default constructor; initial mProductionRule, mCurrentToken, and mCurrentLexeme to empty string.*/
SyntaxAnalyzer::SyntaxAnalyzer()
{
	mProductionRules = "";
	mCurrentToken = "";
	mCurrentLexeme = "";
	
}
/*Overload constructor; receive parameter is a list from lexical analyzer then transfer tokens to mListToken.
- get and pop the first token in list
*/
SyntaxAnalyzer::SyntaxAnalyzer( queue <Token>list )
{
	while (!list.empty())
	{
		mListToken.push(list.front());
		list.pop();
	}

	mCurrentTokenObject = mListToken.front();
	mCurrentToken = mCurrentTokenObject.getToken();
	mCurrentLexeme = mCurrentTokenObject.getLexeme();
	mProductionRules = "";
	mListToken.pop();

	Rat18F();
}

SyntaxAnalyzer::~SyntaxAnalyzer()
{}


/*errorMessage(string typeSyntax, string rules): 
Parameter: string of type sytax error, string of name rule
Print the error line number, token, lexeme, what missing and rule name
Return nothing but back to menu.
*/
void SyntaxAnalyzer::errorMessage(string typeSyntax, string rules)
{
	printListSyntax();
	cerr << "\nError at line: " << mCurrentTokenObject.getLineNum()
		<< "\nToken: " << mCurrentToken
		<< "\nLexeme: " << mCurrentLexeme
		<< "\nMissing: " << typeSyntax
		<< "\nRule: " << rules
		<< endl;

	return;
}
/*nextToken()
Set production rule for current token object and add to final list
Move on to next token in the list and pop old one
Handle error if list empty then print error message
*/
void SyntaxAnalyzer::nextToken()
{ 
	
	mCurrentTokenObject.setProductionRule(mProductionRules);
	mListTokenSyntax.push(mCurrentTokenObject);
	mProductionRules = "";
	if (!mListToken.empty())
	{
		mCurrentTokenObject = mListToken.front();
		mCurrentToken = mCurrentTokenObject.getToken();
		mCurrentLexeme = mCurrentTokenObject.getLexeme();
		mListToken.pop();
	}
	else
		cerr << "Empty List" << endl;
	
}
/*printListSyntax()
Create or open Output file to write out
Print the list syntax token in the console and write out the file
Close the file
*/
void SyntaxAnalyzer::printListSyntax()
{
	fstream outFile;
	outFile.open("Output.txt", ios::out | ios::app);

	cout << "*********************************************************" << endl;
	while (!mListTokenSyntax.empty())
	{
		
		cout << mListTokenSyntax.front();
		outFile <<"Token: " << left << setw(15) << mListTokenSyntax.front().getToken() <<"Lexeme: "<< mListTokenSyntax.front().getLexeme() << endl;
		outFile << mListTokenSyntax.front().getProductionRule() << endl;

		mListTokenSyntax.pop();

	}

	outFile << endl;
	outFile.close();

}

//R1. <Rat18F>  ::=   <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List>  $$
void SyntaxAnalyzer::Rat18F()
{
	mProductionRules += "<Rat18F> -> <Opt Function Definitions>   $$  <Opt Declaration List>  <Statement List> $$\n";
	if (mCurrentLexeme == "function")
	{
		OptFunctionDefinition();
		if (mCurrentLexeme == "$$")
		{
			nextToken();
			OptDeclarationList();
			StatementList();
			if (mCurrentLexeme != "$$")
			{
				errorMessage("$$", "<Rat18F>");
			}
		}
		else
		{
			errorMessage("$$", "<Rat18F>");
		}
	}
	else if (mCurrentLexeme == "$$")
	{
		nextToken();
		OptDeclarationList();
		StatementList();
		if (mCurrentLexeme != "$$")
		{
			errorMessage("$$","<Rat18F>");
		}
	}
	else
	{
		errorMessage("$$ or function", "<Rat18F>");
	}
	
}

//R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty>
void SyntaxAnalyzer::OptFunctionDefinition()
{
	mProductionRules += "<OptFunctionDefinition> -> ";

	if (mCurrentLexeme == "function")
	{
		mProductionRules += "<Function Definitions>\n";
		FunctionDefinition();
	}
	else if (mCurrentLexeme == "$$")
	{
		Empty();
		
	}
	else
	{
		errorMessage("$$", "<OptFunctionDefinition>");
	}
	

}

//R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions>
void SyntaxAnalyzer::FunctionDefinition()
{
	mProductionRules += "<Function Definitions> -> <Function>\n ";
	
		Function();
		if (mCurrentLexeme == "function")
		{
			
			FunctionDefinition();
		}
	
}

//R4. <Function> ::= function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>
void SyntaxAnalyzer::Function()
{
	mProductionRules += "<Function> -> function  <Identifier>   ( <Opt Parameter List> )  <Opt Declaration List>  <Body>\n";
	nextToken();
	if (mCurrentToken == "Identifier")	
	{
		nextToken();
		if (mCurrentLexeme == "(")
		{
			nextToken();
			OptParameterList();

			if (mCurrentLexeme == ")")
			{
				nextToken();
				OptDeclarationList();
				Body();
			}
			else
				errorMessage(")", "<Function>");

		}
		else
			errorMessage("(", "<Function>");

	}
	else
	errorMessage("Identifier", "<Function>");

}

//R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty>
void SyntaxAnalyzer::OptParameterList()
{
	mProductionRules += "<OptParameterList> -> ";

	if (mCurrentToken == "Identifier") 
	{
		mProductionRules += "<Parameter List> \n";
		ParameterList();
	}
	else if (mCurrentLexeme == ")") 
	{
		Empty();
	}
	else 
	{
		errorMessage("Identifier or )","<OptParameterList>");
	}

}

//R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List>
void SyntaxAnalyzer::ParameterList()
{
	mProductionRules += "<ParameterList> -> <Parameter> \n";


	if (mCurrentToken == "Identifier") 
	{
		Parameter();

		if (mCurrentLexeme == ",") 
		{
			nextToken();
			ParameterList();
		}
		
	}
	else
	{
		errorMessage("Identifier", "<ParameterList>");
	}
}

//R7. <Parameter> :: = <IDs > : <Qualifier>
void SyntaxAnalyzer::Parameter()
{
	mProductionRules += "<Parameter> ->  <IDs > : <Qualifier> \n";

	if (mCurrentToken == "Identifier")
	{
		IDs();

		if (mCurrentLexeme == ":")
		{
			nextToken();
			Qualifier();
		}
		else
		{
			errorMessage(":", "<Parameter>");
		}
	}
	else
	{
		errorMessage("Identifier", "<Parameter>");
	}
}

//R8. <Qualifier> ::= int     |    boolean    |  real
void SyntaxAnalyzer::Qualifier()
{
	
	bool isFound = false;
	for (string str : qualifierList)
	{
		if (mCurrentLexeme == str)
		{
			mProductionRules += "Qualifier -> " + str +"\n";
			isFound = true;
		}
	}
	if (!isFound)
	{
		errorMessage("int or boolean or real ","<Qualifier>");
	}

	nextToken();
}

//R9. <Body>  ::=  {  < Statement List>  }
void SyntaxAnalyzer::Body()
{
	mProductionRules += "<Body> -> {  < Statement List>  }\n";

	if (mCurrentLexeme == "{") 
	{
		nextToken();
		StatementList();

		if (mCurrentLexeme == "}")
		{
			nextToken();
		}
		else 
			errorMessage("}","<Body>");

	}
	else 
		errorMessage("{","<Body>");
}

//R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty>
void SyntaxAnalyzer::OptDeclarationList()
{
	mProductionRules += "<OptDeclarationList> -> ";

	if (mCurrentLexeme == "{"|| mCurrentLexeme == "while"|| mCurrentLexeme == "get"|| mCurrentLexeme == "put"|| mCurrentLexeme == "return"
		|| mCurrentLexeme == "return"|| mCurrentToken == "Identifier")
	{
		Empty();
	}
	else if (mCurrentLexeme == "int" || mCurrentLexeme == "boolean" || mCurrentLexeme == "real")
	{
		mProductionRules += "<Declaration List> \n";
		DeclarationList();
	}
	else 
	{
		errorMessage("statements or int, boolean, real","<Opt Declaration List>");
	}
}

//R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List>
void SyntaxAnalyzer::DeclarationList()
{
	mProductionRules += "<DeclarationList> -> <Declaration> ; \n";


	Declaration();

	if (mCurrentLexeme == ";")
	{
		
		nextToken();
		if (mCurrentLexeme == "int" || mCurrentLexeme == "boolean" || mCurrentLexeme == "real")
		{
			
			DeclarationList();
		}
	}
	else 
	{
		errorMessage(";","<DeclarationList>");
	}
}

//R12. <Declaration> ::=   <Qualifier > <IDs>        
void SyntaxAnalyzer::Declaration()
{
	mProductionRules += "<Declaration> -> <Qualifier > <IDs>  \n";
	Qualifier();
	if (mCurrentToken == "Identifier")
	{
		IDs();
	}
	else
	{
		errorMessage("Identifier", "<Declaration>");
	}
}

//R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs>
void SyntaxAnalyzer::IDs()
{
	mProductionRules += "<IDs> -> <Identifier> \n";

	if (mCurrentToken == "Identifier")
	{
		nextToken();

		if (mCurrentLexeme == ",")
		{
			
			nextToken();
			if (mCurrentToken == "Identifier")
			{
				
				IDs();
			}
		}
	}
	else
		errorMessage("Identifier", "<IDs>");
}

//R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List>
void SyntaxAnalyzer::StatementList()
{
	mProductionRules += "<StatementList> -> <Statement> \n";
		Statement();
		if (mCurrentLexeme == "{" || mCurrentLexeme == "if" || mCurrentLexeme == "return"
			|| mCurrentLexeme == "put" || mCurrentLexeme == "get" || mCurrentLexeme == "while"
			|| mCurrentToken == "Identifier")
		{
			StatementList();
		}
	
}

//R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While>
void SyntaxAnalyzer::Statement()
{
	mProductionRules += "<Statement> ->  ";
	
	if (mCurrentToken=="Identifier") 
	{
		mProductionRules += "<Assign>\n";
		Assign();
	}
	else if (mCurrentLexeme == "{") 
	{
		mProductionRules += "<Compound>\n";
		Compound();
	}
	else if (mCurrentLexeme == "if")
	{
		mProductionRules += "<If>\n";
		If();
	}
	else if (mCurrentLexeme == "return")
	{
		mProductionRules += "<Return>\n";
		Return();
	}
	else if (mCurrentLexeme == "put")
	{
		mProductionRules += "<Print>\n";
		Print();
	}
	else if (mCurrentLexeme == "get")
	{
		mProductionRules += "<Scan>\n";
		Scan();
	}
	else if (mCurrentLexeme == "while")
	{
		mProductionRules += "<While>\n";
		While();
	}
	else 
	{
		errorMessage("{, if, return, put, get, while, identifier","<Statement>");
	}
}

//R16. <Compound> ::=   {  <Statement List>  } 
void SyntaxAnalyzer::Compound()
{
	mProductionRules += "<Compound> ->  {  <Statement List>  }\n";

	if (mCurrentLexeme == "{") 
	{
		nextToken();
		StatementList();

		if (mCurrentLexeme == "}") 
		{
			nextToken();
		}
		else 
		{
			errorMessage("}", "<Compound>");
		}
	}
	else
	{
		errorMessage("{", "<Compound>");
	}
}

//R17. <Assign> ::=     <Identifier> = <Expression> ;
void SyntaxAnalyzer::Assign()
{
	mProductionRules += "<Assign> ->   <Identifier> = <Expression> ;\n";
	
	if (mCurrentToken == "Identifier")
	{
		nextToken();
		if (mCurrentLexeme == "=")
		{
			nextToken();
			Expression();
			if (mCurrentLexeme == ";")
			{
				nextToken();
			}
			else
			{
				errorMessage(";", "<Assign>");
			}
		}
		else
		{
			errorMessage("=", "<Assign>");
		}
	}
	else
	{
		errorMessage("Identifier", "<Assign>");
	}
	
}

//R18. <If> ::=     if  ( <Condition>  ) <Statement>   ifend    |   if (<Condition>) < Statement > else  <Statement>  ifend
void SyntaxAnalyzer::If()
{
	mProductionRules += "<If> -> if  ( <Condition>  ) <Statement>  \n";

	if (mCurrentLexeme == "if")
	{
		nextToken();
		if (mCurrentLexeme == "(")
		{
			nextToken();
			Condition();
			if (mCurrentLexeme == ")")
			{
				nextToken();
				Statement();
				if (mCurrentLexeme == "ifend")
				{
					mProductionRules += "ifend";
					nextToken();
				}
				else if (mCurrentLexeme == "else")
				{
					mProductionRules += "else  <Statement>  ifend";
					nextToken();
					Statement();

					if (mCurrentLexeme == "ifend")
					{
						nextToken();
					}
					else 
						errorMessage("ifend","<If>");
					
				}
				else
				{
					errorMessage("ifend or else", "<If>");
				}
				
			}
			else
			{
				errorMessage(")", "<If>");
			}
			
		}
		else
		{
			errorMessage("(", "<If>");
		}
		

	}
	else
	{
		errorMessage("if", "<If>");
	}
}

//R19. <Return> ::=  return ; |  return <Expression> ;
void SyntaxAnalyzer::Return()
{
	if (mCurrentLexeme == "return")
	{
		nextToken();

		if (mCurrentLexeme == "-" || mCurrentToken == "Integer" || mCurrentToken == "Real" || mCurrentLexeme == "("
			|| mCurrentLexeme == "true" || mCurrentLexeme == "false" || mCurrentToken == "Identifier")
		{
			mProductionRules += "<Return> -> return <Expression>;\n";
			Expression();
			if (mCurrentLexeme == ";")
			{
				nextToken();
			}
			else
			{
				errorMessage(";", "<Return>");
			}		
		}
		else
		{
			mProductionRules += "<Return> -> return;\n";
			nextToken();

		}
	}
	else
	{
		errorMessage("return", "<Return>");
	}

	
}

//R20. <Print> ::=    put ( <Expression>);
void SyntaxAnalyzer::Print()
{
	mProductionRules += "<Print> -> put ( <Expression>);\n";
	if (mCurrentLexeme == "put")
	{
		nextToken();

		if (mCurrentLexeme == "(")
		{
			nextToken();
			Expression();
			if (mCurrentLexeme == ")")
			{
				nextToken();

				if (mCurrentLexeme == ";")
				{
					nextToken();
				}
				else
				{
					errorMessage(";", "<Print>");
				}

			}
			else
			{
				errorMessage(")", "<Print>");
			}

		}
		else
		{
			errorMessage("(", "<Print>");
		}
	}
	else
	{
		errorMessage("put", "<Print>");
	}
}

//R21. <Scan> ::=    get ( <IDs> );
void SyntaxAnalyzer::Scan()
{
	mProductionRules += "<Scan> -> get ( <IDs> );\n";
	if (mCurrentLexeme == "get")
	{
		nextToken();

		if (mCurrentLexeme == "(")
		{
			nextToken();
			IDs();

			if (mCurrentLexeme == ")")
			{
				nextToken();

				if (mCurrentLexeme == ";")
				{
					nextToken();
				}
				else
				{
					errorMessage(";", "<Scan>");
				}
			}
			else
			{
				errorMessage(")", "<Scan>");
			}
		}
		else
		{
			errorMessage("(", "<Scan>");
		}
	}
	else
	{
		errorMessage("get", "<Scan>");
	}
}

//R22. <While> ::=  while ( <Condition>  )  <Statement>  whileend
void SyntaxAnalyzer::While()
{
	mProductionRules += "<While> -> while ( <Condition>  )  <Statement>  whileend\n";
	
	if (mCurrentLexeme == "while")
	{
		nextToken();
		if (mCurrentLexeme == "(")
		{
			nextToken();
			Condition();
			if (mCurrentLexeme == ")")
			{
				nextToken();
				Statement();
				if (mCurrentLexeme == "whileend")
				{
					nextToken();
				}
				else
				{
					errorMessage("whileend", "<While>");
				}
					
			}
			else
			{
				errorMessage(")", "<While>");
			}
				
		}
		else
		{
			errorMessage("(", "<While>");
		}
			
		
	}
	else
	{
		errorMessage("while", "<While>");
	}
	
}

//R23. <Condition> ::=     <Expression>  <Relop>   <Expression>
void SyntaxAnalyzer::Condition()
{
	mProductionRules += "<Condition> ->  <Expression>  <Relop>   <Expression>\n";
	if (mCurrentLexeme == "-" || mCurrentToken == "Integer" || mCurrentToken == "Real" || mCurrentLexeme == "("
		|| mCurrentLexeme == "true" || mCurrentLexeme == "false" || mCurrentToken == "Identifier")
	{
		Expression();
		Relop();
		Expression();
	}
	else
	{
		errorMessage("first condition", "<Condition>");
	}
}

//R24. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<     
void SyntaxAnalyzer::Relop()
{
	bool isFound = false;
	for (string str : relopList)
	{
		if (mCurrentLexeme == str)
		{
			mProductionRules += "\n <Relop> -> " + str;
			isFound = true;
		}
	}

	if (!isFound)
	{
		errorMessage(" ==   |   ^=    |   >     |   <    |   =>    |   =< ","<Relop>");
	}

	nextToken();
}

//R25. <Expression>  ::=     <Term><ExpressionPrime>
void SyntaxAnalyzer::Expression()
{
	mProductionRules += "<Expression> -> <Term> <EpressionPrime>\n";
	if (mCurrentLexeme == "-" || mCurrentToken == "Integer" || mCurrentToken == "Real" || mCurrentLexeme == "("
		|| mCurrentLexeme == "true" || mCurrentLexeme == "false" || mCurrentToken == "Identifier")
	{
		Term();
		ExpressionPrime();
	}
	else
	{
		errorMessage("first expression ", "<Expression>");
	}
}

//R26. <ExpressionPrime> ::=  + <Term><ExpressionPrime> | - <Term> <ExpressionPrime>  | <Empty>
void SyntaxAnalyzer::ExpressionPrime()
{
	mProductionRules += "<ExpressionPrime> ->  ";
	
	if (mCurrentLexeme == "==" || mCurrentLexeme == "^=" || mCurrentLexeme == ">"
			|| mCurrentLexeme == "<" || mCurrentLexeme == "=>" || mCurrentLexeme == "=<" || mCurrentLexeme == ")" || mCurrentLexeme == ";")
	{
		Empty();
	}
	else if (mCurrentLexeme == "+" || mCurrentLexeme == "-")
	{
		
			mProductionRules += mCurrentLexeme + " <Term> <ExpressionPrime>\n";
		
		nextToken();
		Term();
		ExpressionPrime();
	}
	else
	{
		
		errorMessage("follow of expression prime", "<ExpressionPrime>");
	}
	

}

//R27. <Term>    ::=       <Factor> <TermPrime>
void SyntaxAnalyzer::Term()
{
	mProductionRules += "<Term> -> <Factor> <TermPrime>\n";
	if (mCurrentLexeme == "-" || mCurrentToken == "Integer" || mCurrentToken == "Real" || mCurrentLexeme == "("
		|| mCurrentLexeme == "true" || mCurrentLexeme == "false" || mCurrentToken == "Identifier")
	{
		Factor();
		TermPrime();
	}
	else
	{
		errorMessage("first term", "<Term>");
	}
}

//R28. <TermPrime> ::= * <Factor> <TermPrime> | / <Factor> <TermPrime> | <Empty>
void SyntaxAnalyzer::TermPrime()
{
	mProductionRules += "<TermPrime> ->  ";
	
	if (mCurrentLexeme == "*" || mCurrentLexeme == "/") 
	{
		mProductionRules += mCurrentLexeme + "<Factor> <TermPrime> \n";
		nextToken();
		Factor();
		TermPrime();
	}
	else if (mCurrentLexeme == "+" || mCurrentLexeme == "-"|| mCurrentLexeme == "==" || mCurrentLexeme == "^=" || mCurrentLexeme == ">"
		|| mCurrentLexeme == "<" || mCurrentLexeme == "=>" || mCurrentLexeme == "=<" || mCurrentLexeme == ")" || mCurrentLexeme == ";")
	{
		Empty();
	}
	else
	{
		errorMessage("follow term prime", " <TermPrime>");
	}
		
}

//R29. <Factor> ::=      -  <Primary>    |    <Primary>
void SyntaxAnalyzer::Factor()
{
	mProductionRules += "<Factor> -> ";
	
	if (mCurrentLexeme == "-")
	{
		mProductionRules += "-  <Primary>\n";
		nextToken();
		Primary();
	}
	else
	{
		mProductionRules += "<Primary>\n";
		Primary();
	}
}

//R30. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |  <Real> | true | false
void SyntaxAnalyzer::Primary()
{
	mProductionRules += "<Primary> -> ";
	
	if (mCurrentToken == "Identifier") 
	{				
		mProductionRules += "<Identifier> \n";
		nextToken();

		if (mCurrentLexeme == "(")
		{
			mProductionRules += " ( <IDs> )\n";
			nextToken();
			IDs();

			if (mCurrentLexeme == ")")
			{
				nextToken();
			}
			else 
				errorMessage(")","<Primary>");
		}
		
	}
	else if (mCurrentLexeme == "(")
	{
		mProductionRules += " ( <Expression> )\n";
		nextToken();
		Expression();

		if (mCurrentLexeme == ")")
		{
			nextToken();
		}
		else 
			errorMessage(")","<Primary>");
	}
	else if (mCurrentToken == "Integer" )
	{
		mProductionRules += " <Integer> \n";
		
		nextToken();
	}
	else if (mCurrentToken == "Real")
	{
		mProductionRules += "<Real> \n";
		nextToken();
	}
	else if (mCurrentLexeme == "true" || mCurrentLexeme == "false")
	{
		mProductionRules += mCurrentLexeme+ "\n";
		nextToken();
	}
	else
	{
		errorMessage("first primary", "<Primary>");
	}
		
	
}

void SyntaxAnalyzer::Empty()
{
	mProductionRules += "<Empty>\n";
}

