/*
 * assign6_test.cpp
 * Tests for assignment 6
 */
#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include <string.h>
using namespace std;

// #define DEBUG

ostream& operator<< (ostream& out, const vector<string>& vs) {
    if(vs.empty())
        return out;

    for(size_t i = 0; i < vs.size()-1; ++i)
        out << "`" << vs.at(i) << "`, ";
    out << "`" << vs.back() << "`";

    return out;
}

enum State { INTEGER, DECIMAL, FRAC, ALPHA, ALPHANUM, OP_PAREN, LT1, LT2, GT1, GT2, SPACE };

bool tokenize(string s, vector<string>& tokens) {
  string token = "";
  State mode = SPACE;

  s.push_back(' ');

  for(char c : s) {
    #ifdef DEBUG
    std::cout << "character: " << c << std::endl;
    std::cout << "mode: " << mode << std::endl;
    std::cout << c << std::endl;
    #endif

    if( (mode == SPACE) ) { // previous character was a space
      #ifdef DEBUG
      std::cout << "entered mode SPACE" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        token.push_back(c);
        mode = INTEGER;
      } else if ( isalpha(c) ) { // if next character is a letter
        // std::cout << "entered case ALPHA" << std::endl;
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        return false; // error
      } else if ( c == '<') { // if next character is a less than
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        return false; // error
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == INTEGER ) { // Previous character was a number
      #ifdef DEBUG
      std::cout << "entered mode INTEGER" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        token.push_back(c);
        mode = INTEGER;
      } else if ( isalpha(c) ) { // if next character is a letter
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        token.push_back(c);
        mode = DECIMAL;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        return false; // error
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == ALPHA || mode == ALPHANUM) { // Previous character was a letter
      #ifdef DEBUG
      std::cout << "entered mode ALPHA/ALPHANUM" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        token.push_back(c);
        mode = ALPHANUM;
      } else if ( isalpha(c) ) { // if next character is a letter
        token.push_back(c);
        mode = ALPHANUM;
      } else if ( c == '.' ) { // if next character is a decimal
        return false;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        return false; // error
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == DECIMAL ) {
      #ifdef DEBUG
      std::cout << "entered mode DECIMAL" << std::endl;
      #endif

      if ( isdigit(c) ) { // if next character is a digit
        token.push_back(c);
        mode = FRAC;
      } else {
        return false;
      }
    } else if ( mode == FRAC ) {
      #ifdef DEBUG
      std::cout << "entered mode FRAC" << std::endl;
      #endif
      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        token.push_back(c);
        mode = FRAC;
      } else if ( isalpha(c) ) { // if next character is a letter
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        return false;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        return false; // error
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == OP_PAREN ) {
      #ifdef DEBUG
      std::cout << "entered mode OP_PAREN" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = FRAC;
      } else if ( isalpha(c) ) { // if next character is a letter
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        return false;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        return false; // error
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == LT1 || mode == LT2 ) {
      #ifdef DEBUG
      std::cout << "entered mode LT1/LT2" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        return false;
      } else if ( isalpha(c) ) { // if next character is a letter
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        return false;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        if( mode == LT2 ) {
          return false;
        }
        token.push_back(c);
        mode = LT2;
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    } else if ( mode == GT1 || mode == GT2 ) {
      #ifdef DEBUG
      std::cout << "entered mode GT1/GT2" << std::endl;
      #endif

      if( isspace(c) ) { // if next character is a space
        tokens.push_back(token);
        token.clear();
        mode = SPACE; 
      } else if ( isdigit(c) ) { // if next character is a digit
        return false;
      } else if ( isalpha(c) ) { // if next character is a letter
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = ALPHA;
      } else if ( c == '.' ) { // if next character is a decimal
        return false;
      } else if ( c == '<') { // if next character is a less than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = LT1;
      } else if ( c == '>') { // if next character is a greater than
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = GT1;
      } else if ( c == '=') { // if next character is a equals
        if( mode == GT2 ) {
          return false;
        }
        token.push_back(c);
        mode = GT2;
      } else if ( strchr("*/+-()", c) ) { // if next character is a +-*/()
        tokens.push_back(token);
        token.clear();
        token.push_back(c);
        mode = OP_PAREN;
      }
    }
  } // end for

  #ifdef DEBUG
  for (string s : tokens) {
    std::cout << s << endl;
  }
  #endif

  return true;
};

bool check_tokenization(string s, vector<string> tokens) {
    vector<string> output;
    bool b = tokenize(s,output);
    std::cout << "checking " << s << endl;

    if(!b) {
        cout << "CHECK: string \"" << s << "\" should tokenize without errors.\n";
        return false;
    }

    if(tokens != output) {
        cout << "CHECK: tokenization of string \"" << s << "\" is incorrect:\n";
        cout << "       Expected: " << tokens << endl;
        cout << "       Found: " << output << endl;
        return false;
    }

    return true;
}

bool check_error(string s) {
    vector<string> output;

    if(tokenize(s,output)) {
        cout << "CHECK: tokenization of string \"" << s << "\" should fail with errors"
                ", but succeeds.\n";
        return false; 
    }

    return true;
}

bool check_success() {
    return check_tokenization("abc",          {"abc"}) &&
           check_tokenization("123",          {"123"}) &&
           check_tokenization("123.456",      {"123.456"}) &&
           check_tokenization("abc123",       {"abc123"}) &&
           check_tokenization("123abc",       {"123", "abc"}) &&
           check_tokenization("()+-*/",       {"(", ")", "+", "-", "*", "/"}) &&
           check_tokenization("  abc  123  ", {"abc", "123"}) &&
           check_tokenization("123.456abc",   {"123.456", "abc"}) &&
           check_tokenization("123<abc",      {"123", "<", "abc"}) &&
           check_tokenization("123<=abc",     {"123", "<=", "abc"}) &&
           check_tokenization("123>abc",      {"123", ">", "abc"}) &&
           check_tokenization("123>=abc",     {"123", ">=", "abc"}) && 
           check_tokenization("<<>><>",       {"<", "<", ">", ">", "<", ">"}) &&
           check_tokenization("abc\n\t123",   {"abc", "123"});
}

bool check_errors() {
    return check_error("123.") &&
           check_error(".123") &&
           check_error("123=456") && 
           check_error("abc.def") &&
           check_error("123..456") &&
           check_error("123.456.789") &&
           check_error("123<==456") &&
           check_error("123>==456") &&
           check_error("   .   ") &&
           check_error("abc=def");
}

int main(int argc, char**) {
    if(argc > 1) {
        while(true) {
            string input;
            cout << "> ";
            getline(cin, input);
            vector<string> tokens;
            bool b = tokenize(input, tokens);

            if(b) {
                cout << "Tokens (" << tokens.size() << "): " << tokens << endl;
            }
            else
                cout << "Error during tokenization" << endl;
        }
    }
    else if(check_success() && check_errors()) 
        cout << "*** All Tests Passed Successfully! ****\n";
}