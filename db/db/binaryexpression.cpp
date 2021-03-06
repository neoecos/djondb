// =====================================================================================
//  Filename:  binaryexpression.cpp
// 
//  Description:  
// 
//  Version:  1.0
//  Created:  04/24/2012 10:42:29 AM
//  Revision:  none
//  Compiler:  gcc
// 
//  Author:  YOUR NAME (), 
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

#include "binaryexpression.h"

#include "expressionresult.h"
#include "util.h"
#include "bson.h"
#include <string>
#include <string.h>
#include <memory>

BinaryExpression::BinaryExpression(FILTER_OPERATORS oper)
   :BaseExpression(ET_BINARY)
{
	_oper = oper;
	_left = NULL;
	_right = NULL;
}

BinaryExpression::BinaryExpression(const BinaryExpression& orig)
	:BaseExpression(ET_SIMPLE)
{
	_oper = orig._oper;
	_left = orig._left;
	_right = orig._right;
}

BinaryExpression::~BinaryExpression() {
	if (_left) delete _left;
	if (_right) delete _right;
}

ExpressionResult* evalEqual(const BSONObj& bson, BaseExpression* left, BaseExpression* right) {
	ExpressionResult* valLeft = left->eval(bson);
	ExpressionResult* valRight= right->eval(bson);
	
	bool result = false;
	if (valLeft->type() != valRight->type()) {
		result = false;
	} else if (((valLeft->type() != ExpressionResult::RT_NULL) && (valRight->type() == ExpressionResult::RT_NULL)) ||
			((valLeft->type() == ExpressionResult::RT_NULL) && (valRight->type() != ExpressionResult::RT_NULL))) {
		result = false;
	} else {

		// the types are ensured to be equal
		switch (valLeft->type()) {
			case ExpressionResult::RT_INT:
				result = ((__int32)*valLeft == (__int32)*valRight);
				break;
			case ExpressionResult::RT_LONG:
			case ExpressionResult::RT_LONG64:
				result = ((__int64)*valLeft == (__int64)*valRight);
				break;
			case ExpressionResult::RT_DOUBLE:
				result = ((double)*valLeft == (double)*valRight);
				break;
			case ExpressionResult::RT_BOOLEAN:
				result = ((bool)*valLeft == (bool)*valRight);
				break;
			case ExpressionResult::RT_PTRCHAR:
				{
					result = ((djondb::string)*valLeft == (djondb::string)*valRight);
					break;
				}
				break;
			case ExpressionResult::RT_STRINGDB:
				{
					std::string leftS = *valLeft;
					std::string rightS = *valRight;
					result = (leftS.compare(rightS) == 0);
				}
				break;
			case ExpressionResult::RT_BSON: 
				{
					BSONObj* bleft = (BSONObj*)*valLeft;
					BSONObj* bright = (BSONObj*)*valRight;
					result = (*bleft == *bright);
					break;
				}
		}
	}

	delete valLeft;
	delete valRight;
	return new ExpressionResult(result);
}

ExpressionResult* evalComparison(const BSONObj& bson, const FILTER_OPERATORS& oper, BaseExpression* left, BaseExpression* right) {
	ExpressionResult* valLeft = left->eval(bson);
	ExpressionResult* valRight= right->eval(bson);

	if (valLeft->type() != valRight->type()) {
		// ERROR types does not match
		delete valLeft;
		delete valRight;

		return new ExpressionResult(false);
	}

	bool resultGreather = false; // this will compare only greather than, and at the end will invert
	// based on the sign
	if ((valLeft->type() != ExpressionResult::RT_NULL) && (valRight->type() == ExpressionResult::RT_NULL)) {
		resultGreather = true;
	} else if (((valLeft->type() == ExpressionResult::RT_NULL) && (valRight->type() != ExpressionResult::RT_NULL))) {
		resultGreather = false;
	} else {
		switch (valLeft->type()) {
			case ExpressionResult::RT_INT:
				resultGreather = ((__int32)*valLeft > (__int32)*valRight);
				break;
			case ExpressionResult::RT_LONG:
			case ExpressionResult::RT_LONG64:
				resultGreather = ((__int64)*valLeft > (__int64)*valRight);
				break;
			case ExpressionResult::RT_DOUBLE:
				resultGreather = ((double)*valLeft > (double)*valRight);
				break;
		}
	}

	ExpressionResult* result = NULL;
	if ((!resultGreather  && (oper == FO_GREATEREQUALTHAN)) || 
			(resultGreather && (oper == FO_LESSEQUALTHAN))) {
		result = evalEqual(bson, left, right);
	} else {
		bool bres;
		if ((oper == FO_LESSTHAN) || (oper == FO_LESSEQUALTHAN)) {
			bres = !resultGreather;
		}else {
			bres = resultGreather;
		}
		result = new ExpressionResult(bres);
	}

	delete valLeft;
	delete valRight;
	return result;
}

ExpressionResult* evalAndOr(const BSONObj& bson, FILTER_OPERATORS oper, BaseExpression* left, BaseExpression* right) {
	std::auto_ptr<ExpressionResult> valLeft(left->eval(bson));
	std::auto_ptr<ExpressionResult> valRight(right->eval(bson));

	if ((valLeft->type() != ExpressionResult::RT_BOOLEAN) || (valRight->type() != ExpressionResult::RT_BOOLEAN)) {
		// ERROR
		return NULL;
	} else {
		bool bleft = (bool)*valLeft;
		bool bright = (bool)*valRight;

		bool bresult;
		switch (oper) {
			case FO_AND:
				bresult = (bleft && bright);
				break;
			case FO_OR:
				bresult = (bleft ||  bright);
				break;
		}
		ExpressionResult* result = new ExpressionResult(bresult);
		return result;
	}
}

ExpressionResult* BinaryExpression::eval(const BSONObj& bson) {

	switch (_oper) {
		case FO_EQUALS:
			return evalEqual(bson, _left, _right);
		case FO_AND:
		case FO_OR:
			return evalAndOr(bson, _oper, _left, _right);
		case FO_LESSTHAN:
		case FO_LESSEQUALTHAN:
		case FO_GREATERTHAN:
		case FO_GREATEREQUALTHAN:
			return evalComparison(bson, _oper, _left, _right);
	}
}

BaseExpression* BinaryExpression::copyExpression() {
	BinaryExpression* result = new BinaryExpression(this->oper());
	if (_left != NULL) {
		result->_left = _left->copyExpression();
	}
	if (_right != NULL) {
		result->_right = _right->copyExpression();
	}
	return result;
}

void BinaryExpression::push(BaseExpression* expression) {

	if (_left == NULL) {
		_left = expression;
	} else if (_right == NULL) {
		_right = expression;
	} else {
		// ERROR
	}
}


FILTER_OPERATORS BinaryExpression::oper() const {
	return _oper;
}

bool BinaryExpression::full() const {
	return ((_left != NULL) && (_right != NULL));
}
