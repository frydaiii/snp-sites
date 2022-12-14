#include "file_handler.h"

class SnpSite {
private:
    string reference_seq;
    int seq_length;
    FileHandler fh;
public:
    SnpSite(string filename);

    int is_unknown(char base);
    string detect_snps();
};