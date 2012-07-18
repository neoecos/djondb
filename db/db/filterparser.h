// =====================================================================================
//  Filename:  filterparser.h
// 
//  Description: This is the definition of the filterparser class which analyze where expressions
//               and execute filters over elements.
// 
//  Version:  1.0
//  Created:  04/24/2012 09:45:47 AM
//  Revision:  none
//  Compiler:  gcc
// 
//  Author:  Juan Pablo Crossley (crossley@gmail.com), 
// 
// License:
// 
// This file is part of the djondb project, for license information please refer to the LICENSE file,
// the application and libraries are provided as-is and free of use under the terms explained in the file LICENSE
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// =====================================================================================

#ifndef INCLUDE_FILTERPARSER_H
#define INCLUDE_FILTERPARSER_H

#include "filterdefs.h"
#include "bson.h"
#include <string>
#include <vector>
#include <list>
#include <exception>

class BaseExpression;
class ExpressionResult;

class Token {
	public:
		enum TOKEN_TYPE {
			TT_NOTTOKEN,
			TT_EXPRESION,
			TT_OPENPARENTESIS,
			TT_CLOSEPARENTESIS,
			TT_CONSTANT,
			TT_EQUALS,
			TT_AND,
			TT_OR,
			TT_LESSTHAN,
			TT_LESSEQUALTHAN,
			TT_GREATERTHAN,
			TT_GREATEREQUALTHAN
		};

	public:
		Token(Token::TOKEN_TYPE type, const std::string& content) {
			_content = new std::string(content);
			_type = type;
		}

		Token(Token::TOKEN_TYPE type) {
			_content = NULL;
			_type = type;
		}

		Token(const Token& orig) {
			if (orig._content != NULL) 
				_content = new std::string(*orig._content);
			else
				_content = NULL;
			_type = orig._type;
		}

		~Token() {
			if (_content != NULL) delete _content;
		}

		Token::TOKEN_TYPE type() { return _type; }
		std::string* content() {
			return _content;
		}

	private:
		std::string* _content;
		Token::TOKEN_TYPE _type;
};

class ParseException: public std::exception {
	public:
		ParseException(int code, const char* error) {
			_errorCode = code;
			_errorMessage = error;
		}

		virtual const char* what() const throw() {
			return _errorMessage;
		}

		int errorCode() const {
			return _errorCode;
		}

	private:
		int _errorCode;
		const char* _errorMessage;
};

class FilterParser {
	public:
		FilterParser(const FilterParser& orig);
		~FilterParser();
		ExpressionResult* eval(const BSONObj& bson);
		
		static FilterParser* parse(const std::string& expression) ;
			//throw (ParseException);

	private:
		FilterParser(const std::string& expression, BaseExpression* root, std::list<Token*> tokens);

	private:
		std::string _expression;
	   BaseExpression* _root;
		std::list<Token*> _tokens;

};
#endif // INCLUDE_FILTERPARSER_H
