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
    // int count_snp = 0, count_sample = 0;
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
                reference_seq[i] = toupper(seq[i]);
            }
            if (reference_seq[i] != 'N' && !is_unknown(seq[i]) && reference_seq[i] != toupper(seq[i])) {
                reference_seq[i] = '>';
            }
        }
        // count_sample++;
        // if (count_sample == 46) {
        //     cout << 46;
        // }
    }
    // FILE *f = fopen("/home/manh/snp-sites-1/sample/reference_seq.aln", "w");
    // fprintf(f, "%s", reference_seq.c_str());
    // fclose(f);
    fh.close();

}

void SnpSite::print_result(string filename) {
    // FILE *f = fopen(filename.c_str(), "w");
    FILE *f = fopen("/home/manh/snp-sites-1/sample/my_code_result.aln", "w");
    // FILE *f = fopen("/home/manh/snp-sites-1/sample/sample_out.aln", "w");
    fh.open(inputfile);

    while (!fh.is_eof()) {
        pair<string, string> sample = fh.next_seq(seq_length);
        string sample_name = sample.first, seq = sample.second;

        fprintf(f, "%s\n", sample_name.c_str());
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
