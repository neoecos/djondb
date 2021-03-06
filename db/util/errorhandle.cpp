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

#include "errorhandle.h"
#include "util.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <string>

char* _lastError;
int _lastErrorCode;

void setLastError(int errorCode, const char* errorDescription, ...) {
    va_list varlist;
    va_start(varlist, errorDescription);

    int len = strlen(errorDescription) * 2;
    _lastError = (char*)malloc(len);
    memset(_lastError, 0, len);
    vsprintf(_lastError, errorDescription, varlist);
    va_end(varlist);

    _lastErrorCode = errorCode;
}

bool errorOcurred() {
    return (_lastError > 0);
}

const char* lastErrorDescription() {
    return _lastError;
}

int lastErrorCode() {
    return _lastErrorCode;
}

void clearError() {
    _lastError = 0;
    _lastErrorCode = 0;
}

