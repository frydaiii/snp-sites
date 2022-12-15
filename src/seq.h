#include "file_handler.h"

class SnpSite {
private:
    string reference_seq, inputfile;
    int seq_length;
    FileHandler fh;
public:
    SnpSite(string _inputfile);

    int is_unknown(char base);
    void detect_snps();
    void print_result(string filename);
};