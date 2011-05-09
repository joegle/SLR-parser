/////////////////////////////////////////////////////////////////////////////////
// Programmer:Joseph Wright
// Class:CMPSC 471
// Assignment: Lab 9
// File:
// O.S.: Linux
// Compiler:g++
// Purpose: SRL(1) parser
//
/////////////////////////////////////////////////////////////////////////////////
// I certify that, this program code is my work.  Others may have assisted me
// with planning and concepts, but the code was written, solely, by me.
//
// I understand that submitting code which is totally or partially the product
// of other individuals is a violation of the Academic Integrity Policy and not
// acceptable ethical conduct. Violations of academic integrity policies will
// be handled as noted in the instructor?s syllabus.
// Falsification of execution results is an inappropriate and unacceptable
// activity. Such violations may result in zero credit for the assignment,
// reduced credit for the assignment, or course failure.
//           
// Sources of logic assistance:
//
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <vector>
#include <string>
#include <utility>
using namespace std;

#define numStates 6
#define numInputs 3

int t(char);
int go(char);


//Example 1, be sure to change the input string

char terminals[numInputs]={'(',')','$'};
string table[numStates][numInputs]=
  { // ( ) $
    {"s:2","r:S~!","r:S~!"},
    {"","","a:"},
    {"s:2","r:S~!","r:S~!"},
    {"","s:4",""},
    {"s:2","r:S~!","r:S~!"},
    {"","r:S~(S)S","r:S~(S)S"}
  };

char nonterminals[1]={'S'};
int gotos[numStates][1]=
  {
    {1},
    {-1},
    {3},
    {-1},
    {5},
    {-1}
  };

  /*

// Example2, besure to change the input
char terminals[numInputs]={'n','+','$'};
string table[numStates][numInputs]=
  {
    {"s:2","",""},
    {"","s:3","a:"},
    {"","r:E~n","r:E~n"},
    {"s:4","",""},
    {"","r:E~E+n","r:E~E+n"}
  };
char nonterminals[1]={'E'};
int gotos[numStates][numInputs]=
  {
    {1},
    {-1},
    {-1},
    {-1},
    {-1},
    {-1}
  };
  */



int main()
{
  char k;
  const int spacing=10;
  int state;
  char in;
  string action;

  vector < pair < char, int > > stack;  
  stack.push_back( make_pair('$',0) );
  cout<<"hello, Press Return to step through parse"<<endl<<endl;

  string input="()()$"; // example 1
  //  string input="n+n+n$"; // example 2

  cout<<"Enter your input string to recognize: ";
  getline(cin,input);
  input+='$';

  // Header
  cout<<setfill('=')<<left<<setw(2*spacing)<<"Stack"
      <<setw(2*spacing)<<"Input"
      <<setw(2*spacing)<<"Action"<<endl<<setfill(' ');

  while(1)
    {
      
      // Display stack
      cout<<endl;
      int u=0;
      for(u=0; u<stack.size(); u++)
	cout<<stack[u].first<< " "<<stack[u].second<< " ";
      for(;u<7;u++)
	cout<<"    ";
      for(int u=input.size(); u<spacing; u++)
	cout<<" ";

      // Display input
      cout<<setw(spacing)<<input;

      // Display the action code to take
      cout<<setw(spacing)<<left<<action;

      state=stack.back().second;

      in=input[0];

      // action is a string in form:
      ///   r:T~!
      ///    ~ is an arrow
      ///    ! is epsilon
      ///    The first char will direct shift, reduce, error, accepting
      action = table[state][t(in)];

      // Stepping pause 
      cin.get();       

      cout<<"=> ";

      // blank/error cell
      if(action=="")
	{
	  cout<<"ERROR input not accepted"<<endl;
	  return 0;
	}
      
      // Accepted cell
      else if(action[0]=='a')
	{
	  cout<<"INPUT STRING ACCEPTED"<<endl;
	  return 0;
	}

      // Reduction
      else if(action[0]=='r')
	{
	  cout<<"Reducing '"<<action.substr(4,action.length()-1)
	      <<"' on the stack to "<<action[2];


	  // Consume the right hand side of the grammer rule off the stack
	  for(int i=action.length()-1; i>3 ; i--)
	    {
	      
	      if(stack.back().first==action[i])
		stack.pop_back();


	      // Epsilons do not consume anything
	      else if(action[i]=='!')
		continue;


	      else
		{
		  cout<<"Error in reduce, expected "
		      <<stack.back().first<<endl;
		  return 0;
		}
	    }

	  cout<<" Then goto state " << gotos[stack.back().second][go(action[2])]<<endl;


	  // Add the rewrite nonterminal and goto the next state
	  //     according to the goto table, which needs the backtracked state
	  //     and nonterminal from rule
	  stack.push_back(
			  make_pair(
				    action[2],
				    gotos[stack.back().second][go(action[2])]));
	}



      // Shift
      else if(action[0]=='s')
	{
	  cout<<"Shifting top of input to stack and moving to state "<<action[2]<<endl;

	  stack.push_back(
			  make_pair(
				    input[0],
				    action[2]-48));

	  input.erase(0,1);
	}
    }

  return 0;
}


// Finds the collum number of a terminal indexed by 'terminals' array
int t(char terminal)
{ // Linear search bad
  for(int i=0; i<numInputs; i++)
    if(terminals[i]==terminal)
      return i;
  cout<<"t(): There was an error in look finding '"<<terminal<<"' in the table."<<endl;
  return -1;
}

// Finds the collumn number of a nonterminal in the goto table, indexed by the 'nonterminals' array
int go(char nterminal)
{ // Linear search bad
  for(int i=0; i<numInputs; i++)
    if(nonterminals[i]==nterminal)
      return i;
  cout<<"go(): There was an error in look finding '"<<nterminal<<"' in the table."<<endl;
  return -1;
}
