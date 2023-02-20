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
    boost::iostreams::filtering_istream instream;
    // ifstream instream;
public:
    InputHandler();

    void open(const char* filename);
    bool assign_next_sample_to(string *name, string *seq);
};
#endif