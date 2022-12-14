#include "seq.h"

SnpSite::SnpSite(string filename) {
    fh = FileHandler(2048);
    fh.open(filename);
}

int SnpSite::is_unknown(char base)
{
    switch (base) {
        case 'N':
        case 'n':
        case '-':
        case '?':
            return 1;
        default:
            return 0;
    }
}

string SnpSite::detect_snps() {
    int seq_length = -1;
    string seq;
    while (seq = fh.next_seq(), seq != "") {
        if (seq_length == -1) {
            seq_length = seq.length();
            reference_seq = string(seq_length, 'N');
        }

        for (int i = 0; i < seq_length; i++) {
            if (reference_seq[i] == '>') {
                continue;
            }
            if (reference_seq[i] == 'N' && !is_unknown(seq[i])) {
                reference_seq[i] = seq[i];
            }
            if (reference_seq[i] != 'N' && !is_unknown(seq[i]) && reference_seq[i] != seq[i]) {
                reference_seq[i] = '>';
            }
        }
    }
    return reference_seq;
}