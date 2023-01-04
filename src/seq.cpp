#include "seq.h"

snp::SnpSite::SnpSite(char* _inputfile) {
    fh = file_handler::FileHandler();
    seq_length = -1;
    num_of_snps = 0;
    inputfile = _inputfile;
}

int snp::SnpSite::is_unknown(char base)
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

void snp::SnpSite::detect_snps() {
    string sample_name, seq;
    this->fh.open(this->inputfile.c_str());
    while (!this->fh.is_eof()) {
        this->fh.assign_next_sample_to(&sample_name, &seq);

        if (seq_length == -1) {
            this->seq_length = seq.length();
            this->reference_seq = string(this->seq_length, 'N');
        }

        for (int i = 0; i < this->seq_length; i++) {
            if (this->reference_seq[i] == '>') {
                continue;
            }
            if (this->reference_seq[i] == 'N' && !is_unknown(seq[i])) {
                this->reference_seq[i] = toupper(seq[i]);
            }
            if (this->reference_seq[i] != 'N' && !is_unknown(seq[i]) && this->reference_seq[i] != toupper(seq[i])) {
                this->reference_seq[i] = '>';
                this->num_of_snps++;
            }
        }
    }
    this->snps_location = new int[this->num_of_snps * sizeof(int)];
    int j = 0;
    for (int i = 0; i < this->seq_length; i++) {
        if (this->reference_seq[i] == '>') {
            this->snps_location[j++] = i;
        }
    }
    this->fh.close();
}

void snp::SnpSite::print_result(char* filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    this->fh.open(this->inputfile.c_str());

    string sample_name, seq;
    while (!this->fh.is_eof()) {
        this->fh.assign_next_sample_to(&sample_name, &seq);

        fprintf(f, "%s\n", sample_name.c_str());
        for (int i = 0; i < this->num_of_snps; i++) {
            fputc(seq[this->snps_location[i]], f);
        }
        fputc('\n', f);
    }
    this->fh.close();
    fclose(f);
}

void snp::SnpSite::clean() {
    delete(this->snps_location);
}
