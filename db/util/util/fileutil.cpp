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

#include "fileutil.h"
#include "stringfunctions.h"
#include "errorHandle.h"
#include "../util.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <sys/types.h>
#ifndef WINDOWS
#include <dirent.h>
#include <errno.h>
#else
using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;
#endif
#include <string.h>
#include <stdlib.h>

using namespace std;

char* readFile(char* fileName) {
    ifstream ifs;

    ifs.open(fileName, ifstream::in);

    stringbuf* buffer = new stringbuf();

    while (ifs.good()) {
        ifs >> buffer;
    }
    ifs.close();

    string str = buffer->str();

    delete(buffer);

    char* result = strcpy(str);

    return result;
}

int writeFile(const std::string& fileName, const std::string& text, bool append) {
    ofstream ofs;

    ios_base::openmode flags;
    if (append) {
        flags = ios_base::app | ios_base::out;
    } else {
        flags = ios_base::out;
    }
    ofs.open(fileName.c_str(),  flags);
    if (ofs.fail() != 0) {
        setLastError(1, (std::string("Failed to open the file: ") + fileName).c_str());
        return 1;
    }

    ofs << text;
    ofs.close();
    return 0;
}

int getdir (const char* dir, vector<char*> &files, const char* extension)
{
	if (!existDir(dir)) {
        setLastError(1, "Error opening the dir: %s", dir);
        return 1;
	}
    int len = strlen(extension) + 2;
    char* fileextension = (char*)malloc(len);
    memset(fileextension, 0, len);
    fileextension[0] = '.';
    strcpy(fileextension+1, extension);

#ifndef WINDOWS
	 DIR *dp;
	 struct dirent *dirp;
    while ((dirp = readdir(dp)) != NULL) {
        char* currentFile (dirp->d_name);
        if (endsWith(currentFile, fileextension)) {
            files.push_back(strcpy(currentFile));
        }
    }
    closedir(dp);
#else
	String^ folder = Marshal::PtrToStringAnsi((IntPtr) (char *) dir);;
   array<String^>^ arrFiles = Directory::GetFiles( folder );
   for (int i=0; i<arrFiles->Length; i++) {
      String^ file = arrFiles[i];
	  char* currentFile = (char*)(void*)Marshal::StringToHGlobalAnsi(file);
      if (endsWith(currentFile, fileextension)) {
		files.push_back(strcpy(currentFile));
	  }
   }
#endif // #ifndef WINDOWS
    free(fileextension);
    return 0;
}

bool existFile(const char* fileName) {
    ifstream ifs;

    ifs.open(fileName, ifstream::in);

    bool exists = true;
    if (ifs.fail()) {
        exists = false;
    }
    ifs.close();
    return exists;
}

bool existDir(const char* dir) {
	bool exists = true;
#ifndef WINDOWS
    DIR *dp;
    if((dp  = opendir(dir)) == NULL) {
        exists = false;
    }
#else 
	String^ folder = Marshal::PtrToStringAnsi((IntPtr) (char *) dir);;
	exists = Directory::Exists(folder);
#endif //#ifndef WINDOWS
    return exists;
}
