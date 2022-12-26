#ifndef SEQ_H_
#include <zlib.h>
#include <string>
#include <tuple>
#include <string.h>

using namespace std;

#define SEQ_H_

class FileHandler {
private:
    // string seq;
    gzFile file;
    unsigned char *buffer;
    int buffer_size, buffer_start, buffer_end;
    bool eof;
public:
    FileHandler();
    FileHandler(int _buffer_size);

    void open(string filename);
    void close();
    int next_char();
    string next_sample_name();
    tuple<string, string> next_seq();
    pair<string, string> next_seq(int seq_length);
    bool is_eof();
    void get_line_and_append_to(string *s);
};

#endif