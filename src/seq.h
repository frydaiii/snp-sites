#include "file_handler.h"
#include <errno.h>
#include <vector>
#include <stdio.h>

class SnpSite {
private:
    string inputfile, reference_seq;
    int seq_length, num_of_snps;
    FileHandler fh;
    int *snps_location;
public:
    SnpSite(char* _inputfile);

    int is_unknown(char base);
    void detect_snps();
    void print_result(char* filename);
    void clean();
};
