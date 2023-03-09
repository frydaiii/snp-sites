#include "snp_sites.h"

SnpSite::SnpSite(char* _inputfile) {
    seq_length = -1;
    num_of_snps = 0;
    inputfile = _inputfile;
}

void SnpSite::open(const char* filename) {
    // check extension of filename
    string filename_str = filename;
    if (filename_str.substr(filename_str.length() - 3) == ".gz") {
        this->instream.reset();
        std::ifstream file(filename);
        this->instream.push(boost::iostreams::gzip_decompressor());
        this->instream.push(boost::iostreams::file_source(filename));
    }
    if (filename_str.substr(filename_str.length() - 4) == ".aln") {
        this->finstream.open(filename);
    }
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

/* 
    Get next sample name and sequence, assign it to *name and *seq.
*/
bool SnpSite::next_sample(string *name, string *seq) {
    name->erase();
    seq->erase();
    char c;
    while (c != '>' && !this->instream.eof() && c != -1) {
        c = this->instream.get();
    } // read until meet sample name
    if (!getline(this->instream, *name)) return false;
    getline(this->instream, *seq, '>');
    return true;
}

void SnpSite::detect_snps() {
    string sample_name, seq;
    this->open(this->inputfile.c_str());
    Vec32c seq_vec, ref_vec, tmp_vec;
    const int vectorsize = 32;
    int datasize, arraysize;
    // round up datasize to nearest higher multiple of vectorsize
    // vector<int> snps_loc;
    while (this->next_sample(&sample_name, &seq)) {
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
                    this->snps_location.push_back(i + j);
            }
        }
    }
    this->num_of_snps = this->snps_location.size();
    // this->snps_location = new int[this->num_of_snps * sizeof(int)];
    // copy(snps_loc.begin(), snps_loc.end(), this->snps_location);
    cout << "----------- num of snps: " << this->num_of_snps << " -----------" << endl;
}

void SnpSite::print_result(char* filename) {
    FILE *f = fopen(filename, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    this->open(this->inputfile.c_str());

    string sample_name, seq;
    while (this->next_sample(&sample_name, &seq)) {
        fprintf(f, ">%s\n", sample_name.c_str());
        for (int i = 0; i < this->num_of_snps; i++) {
            fputc(seq[this->snps_location[i]], f);
        }
        fputc('\n', f);
    }
    fclose(f);
}

string SnpSite::get_reference_seq() {
    return referrence_seq;
}

/* 
    Get next sample name and sequence, assign it to *name and *seq.
*/
bool SnpSite::next_sample_f(string *name, string *seq) {
    name->erase();
    seq->erase();
    char c;
    while (c != '>' && !this->finstream.eof() && c != -1) {
        c = this->finstream.get();
    } // read until meet sample name
    if (!getline(this->finstream, *name)) return false;
    getline(this->finstream, *seq, '>');
    this->finstream.unget();
    return true;
}

streampos SnpSite::nearest_sample(streampos pos) {
    this->finstream.seekg(pos);
    char c = this->finstream.peek();
    while (c != '>' && !this->finstream.eof()) {
        c = this->finstream.get();
    } // read until meet sample name
    // print tellg
    // cout << "nearest_sample: " << this->finstream.tellg() << endl;
    return this->finstream.tellg();
}

void SnpSite::detect_snps_in(streampos start_pos, streampos end_pos) {
    cout << "--------------- begin detect_snps_in ---------------" << start_pos << " " << end_pos << endl;
    string sample_name, seq;
    cout << "next_sample_f: " << this->finstream.tellg() << endl;
    this->finstream.seekg(start_pos);
    cout << "next_sample_f: " << this->finstream.tellg() << endl;
    Vec32c seq_vec, ref_vec, tmp_vec;
    const int vectorsize = 32;
    int datasize, arraysize, seq_length = -1;
    // round up datasize to nearest higher multiple of vectorsize
    // vector<int> snps_loc;
    while (this->finstream.tellg() < end_pos && this->next_sample_f(&sample_name, &seq)) {
        // cout << "sample_name: " << sample_name << endl;
        // cout << "length: " << seq.length() << endl;
        if (seq_length == -1) {
            datasize = seq.length();
            // round up datasize to nearest higher multiple of vectorsize
            arraysize = (datasize + vectorsize - 1) & (-vectorsize);
            seq_length = arraysize;
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
    char * tmp = new char[vectorsize];
    for (int i = 0; i < arraysize; i += vectorsize) {
        int k = i / vectorsize;
        refvecs[k].store(tmp);
        this->referrence_seq.append(tmp, vectorsize);
    }
    // print number of snps
    // int num_of_snps = 0;
    // for (int i = 0; i < this->referrence_seq.length(); i++) {
    //     if (this->referrence_seq[i] == '>') {
    //         num_of_snps++;
    //     }
    // }
    // cout << "num of snps: " << num_of_snps << endl;
}

void detect_snps_mt(const char* inputfile, const char* outputfile) {
    ifstream input;
    input.open(inputfile);
    input.seekg(0, input.end);
    long file_size = input.tellg();
    int num_of_threads = 4;
    long bytes_per_thread = file_size / num_of_threads;
    input.close();
    // SnpSite * snp = new SnpSite((char*)inputfile);
    // snp->open(inputfile);
    // snp->detect_snps_in(0, snp->nearest_sample(1 * bytes_per_thread));

    // process
    vector<thread> threads;
    vector<streampos> start_poses, end_poses;
    vector<SnpSite*> snps;
    for (int i = 0; i < num_of_threads; i++) {
        SnpSite * snp = new SnpSite((char*)inputfile);
        snp->open(inputfile);
        snps.push_back(snp);
        streampos start_pos = snp->nearest_sample(i * bytes_per_thread);
        streampos end_pos = snp->nearest_sample((i + 1) * bytes_per_thread);
        if (i == num_of_threads - 1 || end_pos == -1) {
            end_pos = file_size;
            start_poses.push_back(start_pos);
            end_poses.push_back(end_pos);
            break;
        }
        start_poses.push_back(start_pos);
        end_poses.push_back(end_pos);
    }
    // update threads number
    num_of_threads = start_poses.size();
    for (int i = 2; i < num_of_threads; i++) {
        threads.push_back(thread(&SnpSite::detect_snps_in, snps[i], start_poses[i], end_poses[i]));
    }
    for (int i = 0; i < num_of_threads; i++) {
        threads[i].join();
    }

    // merge
    string reference_seq = snps[0]->get_reference_seq();
    for (int i = 0; i < num_of_threads; i++) {
        for (int j = 0; j < reference_seq.length(); j++) {
            if (reference_seq[j] == 'N' && snps[i]->get_reference_seq()[j] != 'N') {
                reference_seq[j] = snps[i]->get_reference_seq()[j];
            }
            if (reference_seq[j] != '>' && reference_seq[j] != 'N' && snps[i]->get_reference_seq()[j] != 'N' && reference_seq[j] != snps[i]->get_reference_seq()[j]) {
                reference_seq[j] = '>';
            }
        }
    }

    FILE *f = fopen(outputfile, "w");
    if (!f) {
        fprintf(stderr, "ERROR: cannot open %s for writing: %s\n", outputfile, strerror(errno));
        exit(EXIT_FAILURE);
    }
    SnpSite snp((char*)inputfile);
    snp.open(inputfile);

    string sample_name, seq;
    while (snp.next_sample_f(&sample_name, &seq)) {
        fprintf(f, ">%s\n", sample_name.c_str());
        for (int i = 0; i < sample_name.length(); i++) {
            if (reference_seq[i] == '>') {
                fputc(seq[i], f);
            }
        }
        fputc('\n', f);
    }
    fclose(f);
    // todo: del snps
}
