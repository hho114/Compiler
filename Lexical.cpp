#include "Lexical.h"
#include "Token.h"

/*Decleration of token functions and constructor*/


//Check whether string is operator
bool Lexical::isOperator(string token)
{
	for (int i = 0; i < 11; i++) 
	{
		if (token == operators[i]) 
		{			
			return true;
		}
	}
	return false;
}

//Check whether string is separator
bool Lexical::isSeparator(string token)
{
	for (int i = 0; i < 9; i++) 
	{
		if (token == seperators[i]) 
		{
			return true;
		}
	}
	return false;
}

//check whether string is keyword
bool Lexical::isKeyword(string token)
{
	for (int i = 0; i < 14; i++) 
	{
		if (token == keywords[i]) 
		{
			return true;
		}
	}
	return false;
}

//check whether string is real
bool Lexical::isReal(string token)
{
	for (int i = 0; i < token.length(); i++) 
	{		//if decimal found in string, return true
		if (token[i] == '.') 
		{
			return true;
		}
	}
	return false;
}

//check whether first character of string is letter 
bool Lexical::isLetterFirst(string token)
{
	if (isalpha(token[0])) 
	{			
		return true;
	}

	return false;
}

//check input to determine next state of Identifier FSM
int Lexical::nextStateIdFsm(char input) const
{
	

	if (isalpha(input)) 
	{
		return Letter;
	}
	else if (isdigit(input)) 
	{
		return Digit;
	}
	else
	{
		return Unknown;
	}
	
}

//check input to determine next state of digit FSM
int Lexical::nextStateIntRealFsm(char input) const
{
	

	if (isdigit(input)) 
	{
		return DigitReal;
	}
	else if (input == '.') 
	{
		return Decimal;
	}
	else
	{
		return Unknown;
	}

	
}

/*Lexer function to scanner one string line
	  Determine tokens by charactor
	  Put into a new member token
	  Push that member token into queue list*/
void Lexical::lexer(string lineInput)
{
	
	if (mStillInComment) //check if it is still in comment
	{
		commentScanner(lineInput);//using scanner to scan the comment line
	}
	else //else it not comment
	{
		bool terminal = false; //terminal for case stop the loop
		int	currentState = 0; //current state of FSM
		int	currentColumn = 0; // current column of FSM
		int indexChar = 0;
		char currentChar = lineInput[indexChar]; //current single charractor

		//Parses character by character
		while (currentChar != '\0' && !mStillInComment)
		{

			if (currentChar == '[') { //checks whether it is comment character '['
				//scan through the comment line
				while (currentChar != ']' && currentChar != '\0')
				{
					currentChar = lineInput[indexChar];
					indexChar++;
				}

				if (currentChar != ']') //if the comment has not finished then set mStillInComment is true, to keep check comment next line
				{
					mStillInComment = true;
				}
				else
				{
					currentChar = lineInput[indexChar];
				}
			}
			else //else get the token for lexical analyzer
			{
				if (!isspace(currentChar)) //skip the spaces
				{
					currentState = 0;
					string currentToken = ""; //reset current token
					currentToken += currentChar;

					if (isalpha(currentChar) || isdigit(currentChar))//If it is letter or digit
					{
						terminal = false;//reset terminal
						//While it is not space and terminal is true and character is letter or digit or decimal or '$' 
						while (!isspace(currentChar) && !terminal && (isalpha(currentChar) || isdigit(currentChar) || currentChar == '.' || currentChar == '$'))
						{
							//if next charactor is not end line and it is letter or digit or decimal
							if (lineInput[indexChar + 1] != '\0' && (isalpha(lineInput[indexChar + 1]) || isdigit(lineInput[indexChar + 1]) || lineInput[indexChar + 1] == '.'))
							{
								
								currentChar = lineInput[++indexChar];
								currentToken += currentChar;
							}
							else
							{
								terminal = true;//set terminal to true to get current token and stop the loop
							}
						}


						if (isLetterFirst(currentToken))// if it is letter
						{
							if (isKeyword(currentToken))
							{
								mListToken.push(Token("Keyword", currentToken,  mLineNumber, currentState)); //push into queue list
							}
							else
							{
								for (int i = 0; i < currentToken.length();i++)//use FSM to decide if it is accepted value
								{
									currentState = idFsmTable[currentState][nextStateIdFsm(currentToken[i])];
								}

								if (currentState == 1 || currentState == 2)
								{
									mListToken.push(Token("Identifier", currentToken,  mLineNumber, currentState));//push into queue list
								}
								else
								{
									mListToken.push(Token("Unknown", currentToken,  mLineNumber, currentState));//push into queue list									
								}
							}

						}
						else //else it is digit
						{

							for (int i = 0; i < currentToken.length();i++)//use FSM to decide if it is accepted value
							{
								currentState = intRealFsmTable[currentState][nextStateIntRealFsm(currentToken[i])];
							}

							if (currentState == 1)
							{
								mListToken.push(Token("Integer", currentToken,  mLineNumber, currentState));//push into queue list
								
							}
							else if (currentState == 3)
							{
								mListToken.push(Token("Real", currentToken,  mLineNumber, currentState));//push into queue list
								
							}
							else
							{
								mListToken.push(Token("Unknown", currentToken,  mLineNumber, currentState));//push into queue list
								
							}
						}



					}
					else //else it must be separator or operator or unknown
					{
						//***************************************************************************************************************
						if (currentToken == "$" && lineInput[indexChar + 1] == '$')//It might be "$$", so add one more $ to get its token 
						{
							currentChar = lineInput[++indexChar]; //move to next character
							currentToken += currentChar;//add to current token
						}
						else if(currentToken == "=" && 
							(lineInput[indexChar + 1] == '='|| lineInput[indexChar + 1] == '>'||lineInput[indexChar + 1] == '<')) //It might be comparision, so add one more character to get its token
						{
							currentChar = lineInput[++indexChar]; //move to next character
							currentToken += currentChar;//add to current token
						}
						else if (currentToken == "^" && lineInput[indexChar + 1] == '=')//It might be not equal sign, so add one more character to get its token
						{
							currentChar = lineInput[++indexChar]; //move to next character
							currentToken += currentChar;//add to current token
						}
						//*****************************************************************************************************************

						if (isSeparator(currentToken))//if it is Separator
						{
							mListToken.push(Token("Separator", currentToken,  mLineNumber, currentState));//push into queue list

						}
						else if (isOperator(currentToken))//if it is Operator
						{
							mListToken.push(Token("Operator", currentToken,  mLineNumber, currentState));//push into queue list
						}
						else// else it is unkown
						{
							mListToken.push(Token("Unknown", currentToken,  mLineNumber, currentState));//push into queue list
						}


					}

				}
				currentChar = lineInput[++indexChar]; //go to next charactor in string

			}

		}


	}
	
	mLineNumber++; //increase line number counter
}

//Scanner to decide whether comment have done
void Lexical::commentScanner(string lineInput)
{
		int indexChar = 0;
		char currentChar = lineInput[indexChar];
		
			while (currentChar != ']' && currentChar != '\0') // loop continues until the end of line or comment ']'
			{
				currentChar = lineInput[indexChar++];
			}

			if (currentChar == ']')//if it is end of comment set mStillInComment to false
				mStillInComment = false;
			
}


queue<Token> Lexical::getListToken()
{
	return mListToken;
}

Lexical::~Lexical()
{
	if (!mListToken.empty())
	{
		while (!mListToken.empty())
		{
			mListToken.pop();
		}
	}
}






