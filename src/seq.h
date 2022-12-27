#include "file_handler.h"
#include <algorithm>
#include <vector>

class SnpSite {
private:
    string inputfile;
    char *reference_seq;
    int seq_length;
    FileHandler fh;
    vector<int> snps_location;
public:
    SnpSite(string _inputfile);

    int is_unknown(char base);
    void detect_snps();
    void print_result(string filename);
};
