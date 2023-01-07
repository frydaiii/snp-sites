#include "snp_sites.h"

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
    Vec32c seq_vec, ref_vec;
    const int vectorsize = 32;
    int datasize, arraysize;
    // round up datasize to nearest higher multiple of vectorsize
    vector<int> snps_loc;
    while (!this->fh.is_eof()) {
        this->fh.assign_next_sample_to(&sample_name, &seq);

        if (seq_length == -1) {
            this->seq_length = seq.length();
            datasize = seq.length();
            // round up datasize to nearest higher multiple of vectorsize
            arraysize = (datasize + vectorsize - 1) & (-vectorsize);
            this->reference_seq = string(arraysize, 'N');
        }
        // set excess data to 'N'
        seq.append(string(arraysize - datasize, 'N'));

        for (int i = 0; i < arraysize; i += vectorsize) {
            ref_vec.load(reference_seq.c_str() + i);
            seq_vec.load(seq.c_str() + i);
            
            // normalize
            seq_vec = select(seq_vec >= 'a' && seq_vec <= 'z', seq_vec - 32, seq_vec);
            seq_vec = select(seq_vec == '-', 'N', seq_vec);
            seq_vec = select(seq_vec == '?', 'N', seq_vec);

            // process
            ref_vec = select(ref_vec == 'N' && seq_vec != 'N', seq_vec, ref_vec);
            ref_vec = select(ref_vec != '>' && ref_vec != 'N' && seq_vec != 'N' && ref_vec != seq_vec, '>', ref_vec);
            char chars[32];
            ref_vec.store(chars);
            reference_seq.replace(i, 32, chars);
            for (int j = 0; j < vectorsize; j++) {
                if (ref_vec[j] == '>') {
                    snps_loc.push_back(i + j);
                }
            }
        }
    }
    sort(snps_loc.begin(), snps_loc.end());
    this->num_of_snps = snps_loc.size();
    this->snps_location = new int[this->num_of_snps * sizeof(int)];
    copy(snps_loc.begin(), snps_loc.end(), this->snps_location);

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
