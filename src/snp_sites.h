#include "input_handler.h"
#include <errno.h>
#include <cstring>
#include <vector>
#include "vectorclass/vectorclass.h"
#include <algorithm>

class SnpSite {
private:
    string inputfile;
    int seq_length, num_of_snps;
    InputHandler fh;
    vector<int> snps_location;
    vector<Vec32c> refvecs;
public:
    SnpSite(char* _inputfile);

    int is_unknown(char base);
    void detect_snps();
    void print_result(char* filename);
    void assign_next_sample_to(string *name, string *seq);
};
