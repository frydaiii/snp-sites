#ifndef SEQ_H_
#include <zlib.h>
#include <utility>
#include <string>
#include <ctype.h>
#include "my_string.h"

using namespace std;

#define SEQ_H_

class FileHandler {
private:
    gzFile file;
    char buffer[2048]; // Static allocation for better performance.
    int buffer_start, buffer_end;
    bool eof;
public:
    FileHandler();
    FileHandler(int _buffer_size);

    void open(string filename);
    void close();
    void get_until(int delimiter, MyString *s);
    pair<MyString, MyString> next_sample();
    int next_char();
    bool is_eof();
    bool match_delimiter(int delimiter, int c);
};

#endif