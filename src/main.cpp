#include <iostream>
#include <zlib.h>
#include "seq.h"

int main() {
    clock_t start, end;
    start = clock();

    SnpSite snp_site("/home/manht/snp-sites-1/sample/sample.aln");
    // cout << "big file";
    // SnpSite snp_site("/mnt/c/Users/manht/Downloads/s_typhi_wong_holt.aln.gz");
    snp_site.detect_snps();
    snp_site.print_result("/home/manht/snp-sites-1/sample/sample_out.aln");

    end = clock();
    printf("complete in %f secs", ((double) (end - start)) / CLOCKS_PER_SEC);
}