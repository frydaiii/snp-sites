#include "file_handler.h"
#include <algorithm>
#include <vector>
#include <stdio.h>

class SnpSite {
private:
    char* inputfile;
    char *reference_seq;
    int seq_length;
    FileHandler fh;
    vector<int> snps_location;
public:
    SnpSite(char* _inputfile);

    int is_unknown(char base);
    void detect_snps();
    void print_result(char* filename);
};
