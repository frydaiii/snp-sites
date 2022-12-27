#include <iostream>
#include <zlib.h>
#include "seq.h"

int main() {
    clock_t start, end;
    start = clock();

    // SnpSite snp_site("/home/manht/snp-sites-1/sample/sample.aln");
    // SnpSite snp_site("/mnt/c/Users/Teko/Downloads/s_typhi_wong_holt.aln");
    SnpSite snp_site("/mnt/c/Users/manht/Downloads/s_typhi_wong_holt.aln.gz");
    snp_site.detect_snps();
    end = clock();
    printf("complete detect %f secs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    snp_site.print_result("");

    end = clock();
    printf("complete in %f secs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}