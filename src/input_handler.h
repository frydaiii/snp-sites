#ifndef SEQ_H_
#include <zlib.h>
#include <utility>
#include <ctype.h>
#include <string>
#include <fstream>
#include <iostream>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>

using namespace std;

#define SEQ_H_

#define BUFFER_SIZE 2048

typedef bool (*match_func)(int c, int delimiter);

class InputHandler {
private:
    gzFile file;
    char buffer[BUFFER_SIZE]; // Static allocation for better performance.
    int buffer_start, buffer_end;
    bool eof;
    void get_until(int delimiter, string *s);
    boost::iostreams::filtering_istream instream;
    // ifstream instream;
public:
    InputHandler();

    void open(const char* filename);
    void close();
    pair<string*, string*> next_sample();
    bool assign_next_sample_to(string *name, string *seq);
    int next_char();
    bool is_eof();
};
#endif