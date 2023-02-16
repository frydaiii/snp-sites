#include <iostream>
#include "snp_sites.h"

int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();

    // check to see if the input alignment file exists
    // argv[1] = "/mnt/c/Users/Teko/Downloads/s_typhi_wong_holt.aln.gz";
    argv[1] = "/home/manh/snp-sites-1/sample/sample.aln.gz";
    argv[2] = "/home/manh/snp-sites-1/sample/my_code_result.aln";
    // argv[1] = "/mnt/c/Users/manht/Downloads/s_typhi_wong_holt.aln.gz";
    // argv[2] = "/home/manht/snp-sites-1/sample/their_result.aln";

    if (access(argv[1], F_OK) == -1) {
        fprintf(stderr, "ERROR: cannot access input alignment file '%s'\n", argv[1]);
        fflush(stderr);
        exit(EXIT_FAILURE);
    }
    SnpSite snp_site(argv[1]);
    snp_site.detect_snps();
    end = clock();
    printf("complete detect %f secs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    snp_site.print_result(argv[2]);
    snp_site.clean();

    end = clock();
    printf("complete in %f secs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}