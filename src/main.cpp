#include <iostream>
#include <zlib.h>
#include "seq.h"

using namespace std;

int main() {
    FileHandler fh;
    fh.open("/home/manh/snp-sites/paper_supporting_material/small.aln");

    string seq;
    while (seq = fh.next_seq(), seq != "") {
        cout << seq;
    }
}