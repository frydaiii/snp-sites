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
        sample = fh.next_seq(seq_length);
        char *sample_name = sample.first.c_str(), *seq = sample.second.c_str();

        // if (strstr(sample_name, "Salmonella_enterica_subsp_enterica_serovar_Typhi_str_P_stx_12_v1") != NULL) {
        //     /* ... */
        //     int i;
        //     i = 0;
        // }
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
    FILE *f = fopen("/home/manht/snp-sites-1/sample/my_code_result.aln", "w");
    // FILE *f = fopen("/home/manht/snp-sites-1/sample/sample_out.aln", "w");
    fh.open(inputfile);

    while (!fh.is_eof()) {
        pair<MyString, MyString> sample = fh.next_seq(seq_length);
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
