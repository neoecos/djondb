#include "fileinputoutputstream.h"

#include <string.h>
#include <boost/crc.hpp>
#include <stdlib.h>
#include <sstream>

FileInputOutputStream::FileInputOutputStream(std::string fileName, const char* flags) {
    _pFile = fopen(fileName.c_str(), flags);

    // Position the cursor at the end of the file
    fseek(_pFile, 0, SEEK_END);
    _fileName = fileName;
}

FileInputOutputStream::~FileInputOutputStream() {
    close();
}

/* Write 1 byte in the output */
void FileInputOutputStream::writeChar (unsigned char v)
{
    fwrite(&v, 1, 1, _pFile);
}

/* Write 2 bytes in the output (little endian order) */
void FileInputOutputStream::writeInt (int v)
{
    unsigned char c = (v & 255);
    unsigned char c2= ((v >> 8) & 255);
    writeChar (c);
    writeChar (c2);
}

/* Write 4 bytes in the output (little endian order) */
void FileInputOutputStream::writeLong (long v)
{
    writeInt ((v) & 0xffff);
    writeInt ((v >> 16) & 0xffff);
}

/* Write a 4 byte float in the output */
void FileInputOutputStream::writeFloatIEEE (float v)
{
    fwrite(&v, 1, sizeof(v), _pFile);
}

/* Write a 8 byte double in the output */
void FileInputOutputStream::writeDoubleIEEE (double v)
{
    fwrite(&v, 1, sizeof(v), _pFile);
}

void FileInputOutputStream::writeChars(const char *text, int len) {
    writeInt(len);
    fwrite(text, 1, len, _pFile);
}

void FileInputOutputStream::writeString(const std::string* text) {
    if (text == NULL) {
        writeChars("", 0);
    } else {
        const char* c = text->c_str();
        int l = strlen(c);
        writeChars(c, l);
    }
}

void FileInputOutputStream::seek(long i) {
    fflush(_pFile);
    fseek (_pFile, i, SEEK_SET);
}

long FileInputOutputStream::currentPos() const {
    return ftell(_pFile);
}

void FileInputOutputStream::close() {
    if (_pFile) {
        flush();
        fclose(_pFile);
        _pFile = 0;
    }
}

void FileInputOutputStream::flush() {
    fflush(_pFile);
}

const std::string FileInputOutputStream::fileName() const {
    return _fileName;
}

unsigned char FileInputOutputStream::readChar() {
    unsigned char v;
    fread(&v, 1, 1, _pFile);
    return v;
}

/* Reads 2 bytes in the input (little endian order) */
int FileInputOutputStream::readInt () {
    int v = readChar() | readChar() << 8;
    return v;
}

/* Reads 4 bytes in the input (little endian order) */
long FileInputOutputStream::readLong () {
    long v = readInt() | readInt() << 16;

    return v;
}

/* Reads a 4 byte float in the input */
float FileInputOutputStream::readFloatIEEE () {
    float f;
    fread(&f, 1, sizeof(f), _pFile);
    return f;
}

/* Reads a 8 byte double in the input */
double FileInputOutputStream::readDoubleIEEE () {
    double d;
    fread(&d, 1, sizeof(d), _pFile);
    return d;
}

/* Read a chars */
char* FileInputOutputStream::readChars() {
    int len = readInt();
    char* res = readChars(len);
    return res;
}

std::string* FileInputOutputStream::readString() {
    char* c = readChars();
    std::string* res = new std::string(c);
    free(c);
    return res;
}

char* FileInputOutputStream::readChars(int length) {
    char* res = (char*)malloc(length+1);
    memset(res, 0, length+1);
    fread(res, 1, length, _pFile);
    return res;
}

const char* FileInputOutputStream::readFull() {
    fseek(_pFile, 0, SEEK_SET);
    std::stringstream ss;
    char buffer[1024];
    int readed = 0;
    while (!feof(_pFile)) {
        memset(buffer, 0, 1024);
        readed = fread(buffer, 1, 1023, _pFile);
        ss << buffer;
    }
    std::string str = ss.str();
    return strdup(str.c_str());
}

bool FileInputOutputStream::eof() {
    if (_pFile == NULL) {
        return true;
    }
    long pos = currentPos();
    // Force reading the last char to check the feof flag
    readChar();
    bool res = feof(_pFile);
    // Back to the original position
    seek(pos);
    return res;
}
