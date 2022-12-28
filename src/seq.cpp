#include "seq.h"

SnpSite::SnpSite(char* _inputfile) {
    fh = FileHandler();
    seq_length = -1;
    num_of_snps = 0;
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
    while (!fh.is_eof()) {
        sample = fh.next_sample();
        char *sample_name = sample.first.c_str(), *seq = sample.second.c_str();

        if (seq_length == -1) {
            seq_length = sample.second.length();
            reference_seq = new char[seq_length * sizeof(char)];
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
                num_of_snps++;
            }
        }
        delete(sample_name);
        delete(seq);
    }
    snps_location = new int[num_of_snps * sizeof(int)];
    int j = 0;
    for (int i = 0; i < seq_length; i++) {
        if (reference_seq[i] == '>') {
            snps_location[j++] = i;
        }
    }
    fh.close();
}

void SnpSite::print_result(char* filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    fh.open(inputfile);

    while (!fh.is_eof()) {
        pair<MyString, MyString> sample = fh.next_sample();
        char *sample_name = sample.first.c_str(), *seq = sample.second.c_str();

        fprintf(f, "%s\n", sample_name);
        for (int i = 0; i < num_of_snps; i++) {
            fputc(seq[snps_location[i]], f);
        }
        fputc('\n', f);
        delete(sample_name);
        delete(seq);
    }

    delete(snps_location);
    fh.close();
    fclose(f);
}
