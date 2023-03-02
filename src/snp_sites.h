// #include "input_handler.h"
#include <errno.h>
#include <cstring>
#include <vector>
#include "vectorclass/vectorclass.h"
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/device/file.hpp>

using namespace std;

class SnpSite {
private:
    string inputfile;
    int seq_length, num_of_snps;
    vector<int> snps_location;
    vector<Vec32c> refvecs;
    boost::iostreams::filtering_istream instream;

    // multithread variables
    ifstream finstream;
    vector<string> reference_seqs;
public:
    SnpSite(char* _inputfile);

    void open(const char* filename);
    int is_unknown(char base);
    void detect_snps();
    void print_result(char* filename);
    bool next_sample(string *name, string *seq);

    // multithread functions
    void detect_snps_mt();
    void detect_snps_in(streampos start_pos, streampos end_pos);
    streampos nearest_sample(streampos pos);
};
