#ifndef SEQ_H_
#include <zlib.h>
#include <string>

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
    string next_seq();
    bool is_eof();
};

#endif