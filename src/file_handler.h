#ifndef SEQ_H_
#include <zlib.h>
#include <utility>
#include <string>
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
    int next_char();
    string next_sample_name();
    pair<MyString, MyString> next_seq();
    bool is_eof();
    void get_line_and_append_to(MyString *s);
};

#endif