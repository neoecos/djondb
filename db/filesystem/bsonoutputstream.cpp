// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
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
// *********************************************************************************************************************

#include "bsonoutputstream.h"

#include "outputstream.h"
#include "bson.h"
#include "util.h"
#include <string.h>

BSONOutputStream::BSONOutputStream(OutputStream* out)
{
    _outputStream = out;
    //ctor
}

BSONOutputStream::~BSONOutputStream()
{
    //dtor
}


void BSONOutputStream::writeBSON(const BSONObj& bson) {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("BSONOutputStream::writeBSON bson elements: %d", bson.length());
	_outputStream->writeLong(bson.length());
	for (std::map<std::string, BSONContent* >::const_iterator i = bson.begin(); i != bson.end(); i++) {
		std::string key = i->first;
		if (log->isDebug()) log->debug("BSONOutputStream::writeBSON name: %s", key.c_str());
		_outputStream->writeString(key);
		BSONContent* cont = i->second;
		// If the type is PTRCHAR_TYPE change it to string_type, to remove this type in future
		_outputStream->writeLong(cont->type() != PTRCHAR_TYPE? cont->type(): STRING_TYPE);
		BSONObj* inner;
		switch (cont->type()) {
			case BSON_TYPE: {
									 BSONContentBSON* bbson = (BSONContentBSON*)cont;
									 inner = (BSONObj*)*bbson;
									 writeBSON(*inner); 
									 break;
								 }
			case INT_TYPE: {
									BSONContentInt* bint = (BSONContentInt*)cont;
									_outputStream->writeInt(*bint);
									break;
								}
			case LONG_TYPE:
			case LONG64_TYPE: {
										BSONContentLong* blong = (BSONContentLong*)cont;
										_outputStream->writeLong(*blong);
										break;
									}
			case DOUBLE_TYPE: {
										BSONContentDouble* bdouble = (BSONContentDouble*)cont;
										_outputStream->writeDoubleIEEE(*bdouble);
										break;
									}
			case STRING_TYPE:
			case PTRCHAR_TYPE: {
										 BSONContentString* bstring = (BSONContentString*)cont;
										 djondb::string str = *bstring;
										 const char* text = str.c_str(); 
										 __int32 len = str.length();
										 _outputStream->writeChars(const_cast<char*>(text), len);
										 break;
									 }
			case BSONARRAY_TYPE: 
									 {
										 BSONContentBSONArray* barray = (BSONContentBSONArray*)cont;
										 BSONArrayObj* array = (BSONArrayObj*)*barray;
										 writeBSONArray(array);
										 break;
									 }
		}
	}
}

void BSONOutputStream::writeBSONArray(const BSONArrayObj* array) {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("BSONOutputStream::writeBSONArray elements: %d", array->length());
	__int32 length = array->length();
	_outputStream->writeLong(length);
	for (__int32 x = 0; x < length; x++) {
		BSONObj* obj = array->get(x);
		writeBSON(*obj);
	}
}

void BSONOutputStream::writeBSONArray(const std::vector<BSONObj*>& array) {
	Logger* log = getLogger(NULL);
	if (log->isDebug()) log->debug("BSONOutputStream::writeBSONArray elements: %d", array.size());
	_outputStream->writeLong(array.size());
	for (std::vector<BSONObj*>::const_iterator i = array.begin(); i != array.end(); i++) {
		BSONObj* obj = *i;
		writeBSON(*obj);
	}
}
