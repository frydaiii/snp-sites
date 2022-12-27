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
    pair<MyString, MyString> sample;
    // int count_snp = 0, count_sample = 0;
    while (!fh.is_eof()) {
        sample = fh.next_sample();
        char *sample_name = sample.first.c_str(), *seq = sample.second.c_str();

        if (seq_length == -1) {
            seq_length = sample.second.length();
            reference_seq = (char*) malloc(seq_length);
            memset(reference_seq, 'N', seq_length);
        }

        for (int i = 0; i < seq_length; i++) {
            if (reference_seq[i] == '>') {
                continue;
            }
            if (reference_seq[i] == 'N' && !is_unknown(seq[i])) {
                reference_seq[i] = toupper(seq[i]);
            }
            if (reference_seq[i] != 'N' && !is_unknown(seq[i]) && reference_seq[i] != toupper(seq[i])) {
                reference_seq[i] = '>';
            }
        }
        free(sample_name);
        free(seq);
    }
    fh.close();

}

void SnpSite::print_result(string filename) {
    FILE *f = fopen("/home/manht/snp-sites-1/sample/my_code_result.aln", "w");
    // FILE *f = fopen("/home/manht/snp-sites-1/sample/sample_out.aln", "w");
    fh.open(inputfile);

    while (!fh.is_eof()) {
        pair<MyString, MyString> sample = fh.next_sample();
        char *sample_name = sample.first.c_str(), *seq = sample.second.c_str();

        fprintf(f, "%s\n", sample_name);
        for (int i = 0; i < seq_length; i++) {
            if (reference_seq[i] == '>') {
                fputc(seq[i], f);
            }
        }
        fputc('\n', f);
        free(sample_name);
        free(seq);
    }

    fh.close();
    fclose(f);
}
