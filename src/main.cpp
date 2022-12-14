#include <iostream>
#include <zlib.h>
#include "seq.h"

using namespace std;

int main() {
    clock_t start, end;
    // time begin
    start = clock();
    // FileHandler fh;
    // fh.open("/home/manh/snp-sites/paper_supporting_material/small.aln");

    // string seq;
    // while (seq = fh.next_seq(), seq != "") {
    //     cout << seq << "\n";
    // }

    SnpSite snp_site("/home/manh/snp-sites-1/sample/sample.aln");
    // SnpSite snp_site("/mnt/c/Users/Teko/Downloads/s_typhi_wong_holt.aln.gz");
    cout << snp_site.detect_snps();

    end = clock();
    printf("complete in %f secs", ((double) (end - start)) / CLOCKS_PER_SEC);
}