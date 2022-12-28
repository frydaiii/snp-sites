#include <iostream>
#include "seq.h"

int main(int argc, char *argv[]) {
    clock_t start, end;
    start = clock();

    // check to see if the input alignment file exists
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

    end = clock();
    printf("complete in %f secs\n", ((double) (end - start)) / CLOCKS_PER_SEC);
}