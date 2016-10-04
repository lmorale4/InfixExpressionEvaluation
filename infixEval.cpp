/*
  Linda Morales
  Assignment 5
  infixEval.cpp
*/
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <string>

using namespace std;

void processFile(ifstream& file);
// input: a file to read
// output: none
// side effects; iterates through the lines in the file and
//   enqueues tokens

void enqueuePostfix(string line, queue<string> &q, stack<string> &s);
// input: two strings, a queue and a stack
// output: a queue
// side effects: enqueues tokens in queue and returns it

void operatorOrder(string token, queue<string> &q, stack<string> &s);
// input: a string, a queue and a stack
// output: none
// side effects: determines order of operators

bool checkPrecedence(string token, stack<string> &s);
// input: a string and a stack
// output: if token has lower or equal pecedence to the top of the stack
// side effects: assigns ranks to operators and checks for precedence

int rankPrecedence(string token);
// input: a string 
// output: an integer rank
// side effects: sets an integer rank to operators and returns it

int evaluatePostfix(string &postfix,queue<string> &q, stack<int> &s);
// input: a string, a queue and a string
// output: a stack
// side effects: evaluates postfix expression

void performOperation(string token, stack<int> &s);
// input: a string and a Stack
// output: interger
// side effects: perfoms operation and pushes the result to the stack

string getInfix(string line);
// input: a string
// output: a string
// side effects: gets infix expression from line in file and gets rid of the #

string charToStr(char c);
// input: a char
// output: a string
// side effects: converts a char into a string

void writeToFile(ofstream& r,string line, string postfix, int eval);
// input: a file to write in, a string and a Stack
// output: none
// side effects; writes items in stack to a file

int main(){
	string userStr;
	cout << "What is the name of your infix file? " << endl;
	cin >> userStr;
	ifstream userFile(userStr.c_str());
  
  if(userFile){
			processFile(userFile);
  }
  userFile.close();
}

void processFile(ifstream& file){
	ofstream result;
  result.open("infixResults.txt");
	string line;
	
  while(getline(file, line)){
    if(line.length()>0){;
			queue<string> q;
      stack<string> iStack;
			enqueuePostfix(line, q, iStack);
      while(iStack.size() != 0){
        string op = iStack.top();
        q.push(op);
        iStack.pop();
      }
			string postfix;
      stack<int> pStack;
      int eval = evaluatePostfix(postfix, q, pStack);
			writeToFile(result, line, postfix, eval);
    }
	}
	result.close();
}


void enqueuePostfix(string line, queue<string> &q, stack<string> &s){
	string token;
	istringstream str(line);
	
	while(str){
		str >> token;
		if(token != "#"){
			if(isdigit(token[0])){
				q.push(token);
			}
			else{
				operatorOrder(token, q, s);
			}
		}
	}
}

void operatorOrder(string token, queue<string> &q, stack<string> &s){
  if(token == "("){
    s.push(token);
  }
  else if(token == ")"){
    while(s.top() != "("){
      string op = s.top();
      q.push(op);
      s.pop();
    }
    s.pop();
  }
  else{
    while(s.size() != 0 && checkPrecedence(token, s)){
      string op = s.top();
      q.push(op);
      s.pop();
    }
    s.push(token);
  }
}

bool checkPrecedence(string token, stack<string> &s){
  int op1 = rankPrecedence(token);
  int op2 = rankPrecedence(s.top());
  return op1 <= op2;
}

int rankPrecedence(string token){
  int rank;
  if(token == "+" || token == "-"){
    rank = 1;
  }
  else if(token == "*" || token == "/"){
    rank = 2;
  }
  else{
    rank = 0;
  }
  return rank;
}

int evaluatePostfix(string &postfix, queue<string> &q, stack<int> &s){
  while(!q.empty()){
    string token = q.front();
    postfix += token + " ";
    q.pop();
    if(isdigit(token[0])){
      s.push(stoi(token));
    }
    else{
      performOperation(token, s);
    }
  }
	return s.top();
}

void performOperation(string token, stack<int> &s){
  int num2 = s.top();
  s.pop();
  int num1 = s.top();
  s.pop();
  int result;
  if(token == "+"){
    result = num1 + num2;
  }
  else if(token == "-"){
    result = num1 - num2;
  }
  else if(token == "*"){
    result = num1 * num2;
  }
  else if(token == "/"){
    result = num1 / num2;
  }
  s.push(result);
}

string getInfix(string line){
  string infix;
  int i = 0;
  while(line[i] != '#' && i< line.length()){
    infix += charToStr(line[i]);
    i++;
  }
  return infix;
}

void writeToFile(ofstream& r,string line, string postfix, int eval){
	string infix = getInfix(line);
	r << "The postfix expression of "<< infix << " is:"<< endl;
  r << postfix <<" which evaluates to "<< eval <<"\n";
  r <<"\n";
}

string charToStr(char c){
  stringstream ss;
  string str;
  ss << c;
  ss >> str;
  return str;
}