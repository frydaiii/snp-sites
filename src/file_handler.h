#ifndef SEQ_H_
#include <zlib.h>
#include <string>
#include <tuple>
#include <string.h>

using namespace std;

#define SEQ_H_

class MyString {
private:
    int len, cap;
    char *s;
public:
    MyString();

    void append(char *_s, int _len);
    char* c_str();
    int length();
    void clean();
};

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
    tuple<string, string> next_seq();
    pair<MyString, MyString> next_seq(int seq_length);
    bool is_eof();
    void get_line_and_append_to(MyString *s);
};

#endif