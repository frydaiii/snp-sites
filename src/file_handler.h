#ifndef SEQ_H_
#include <zlib.h>
#include <utility>
#include <ctype.h>
#include "my_string.h"

using namespace std;

#define SEQ_H_

typedef bool (*match_func)(int c, int delimiter);

class FileHandler {
private:
    gzFile file;
    char buffer[2048]; // Static allocation for better performance.
    int buffer_start, buffer_end;
    bool eof;
    void get_until(int delimiter, MyString *s);
public:
    FileHandler();
    FileHandler(int _buffer_size);

    void open(char* filename);
    void close();
    pair<MyString, MyString> next_sample();
    int next_char();
    bool is_eof();
};

#endif