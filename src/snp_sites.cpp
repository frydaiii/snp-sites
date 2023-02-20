#include "snp_sites.h"

SnpSite::SnpSite(char* _inputfile) {
    InputHandler fh();
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
    string sample_name, seq;
    this->fh.open(this->inputfile.c_str());
    Vec32c seq_vec, ref_vec, tmp_vec;
    const int vectorsize = 32;
    int datasize, arraysize;
    // round up datasize to nearest higher multiple of vectorsize
    vector<int> snps_loc;
    while (this->fh.assign_next_sample_to(&sample_name, &seq)) {
        if (this->seq_length == -1) {
            datasize = seq.length();
            // round up datasize to nearest higher multiple of vectorsize
            arraysize = (datasize + vectorsize - 1) & (-vectorsize);
            this->seq_length = arraysize;
            for (int i = 0; i < arraysize; i += vectorsize) {
                refvecs.push_back(Vec32c('N'));
            }
        }
        // set excess data to 'N'
        seq.append(string(arraysize - datasize, 'N'));

        for (int i = 0; i < arraysize; i += vectorsize) {
            // ref_vec.load(reference_seq.c_str() + i);
            int k = i / vectorsize;
            seq_vec.load(seq.c_str() + i);
            
            // normalize
            seq_vec = select(seq_vec >= 'a' && seq_vec <= 'z', seq_vec - 32, seq_vec);
            seq_vec = select(seq_vec == '-', 'N', seq_vec);
            seq_vec = select(seq_vec == '?', 'N', seq_vec);

            // process
            refvecs[k] = select(refvecs[k] == 'N' && seq_vec != 'N', seq_vec, refvecs[k]);
            refvecs[k] = select(refvecs[k] != '>' && refvecs[k] != 'N' && seq_vec != 'N' && refvecs[k] != seq_vec, '>', refvecs[k]);
        }
    }
    for (int i = 0; i < arraysize; i += vectorsize) {
        int k = i / vectorsize;
        for (int j = 0; j < vectorsize; j++) {
            if (refvecs[k][j] == '>') {
                    snps_loc.push_back(i + j);
            }
        }
    }
    this->num_of_snps = snps_loc.size();
    this->snps_location = new int[this->num_of_snps * sizeof(int)];
    copy(snps_loc.begin(), snps_loc.end(), this->snps_location);

    this->fh.close();
}

void SnpSite::print_result(char* filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    this->fh.open(this->inputfile.c_str());
    // this->fh.back2begin();

    string sample_name, seq;
    while (this->fh.assign_next_sample_to(&sample_name, &seq)) {
        fprintf(f, "%s\n", sample_name.c_str());
        for (int i = 0; i < this->num_of_snps; i++) {
            fputc(seq[this->snps_location[i]], f);
        }
        fputc('\n', f);
    }
    this->fh.close();
    fclose(f);
}

void SnpSite::clean() {
    delete(this->snps_location);
}
