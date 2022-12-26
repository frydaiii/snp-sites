#include "seq.h"

SnpSite::SnpSite(string _inputfile) {
    fh = FileHandler();
    seq_length = -1;
    inputfile = _inputfile;
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

void SnpSite::detect_snps() {
    fh.open(inputfile);
    pair<string, string> sample;
    while (!fh.is_eof()) {
        sample = fh.next_seq(seq_length);
        string sample_name = sample.first, seq = sample.second;

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
    fh.close();
}

void SnpSite::print_result(string filename) {
    // FILE *f = fopen(filename.c_str(), "w");
    FILE *f = fopen("/home/manh/snp-sites-1/sample/my_code_result.aln", "w");
    // FILE *f = fopen("/home/manh/snp-sites-1/sample/sample_out.aln", "w");
    fh.open(inputfile);

    while (!fh.is_eof()) {
        string sample_name, seq;
        tie(sample_name, seq) = fh.next_seq(-1);

        fprintf(f, ">%s\n", sample_name.c_str());
        for (int i = 0; i < seq_length; i++) {
            if (reference_seq[i] == '>') {
                fputc(seq[i], f);
            }
        }
        fputc('\n', f);
    }

    fh.close();
    fclose(f);
}
