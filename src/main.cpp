#include <iostream>
#include <zlib.h>
#include <kseq.h>

KSEQ_INIT(gzFile, gzread)

using namespace std;

int main() {
    gzFile fp;
    kseq_t *seq;

    fp = gzopen("/mnt/c/Users/Teko/Downloads/s_typhi_wong_holt.aln.gz", "r");
    seq = kseq_init(fp);
}