#include "file_handler.h"
#include <errno.h>
#include <cstring>
#include <vector>
#include "vectorclass/vectorclass.h"
#include <algorithm>

// Define function type
// Change this to fit the entry function. Should not contain vector types:
typedef int snpFuncType(char *input_file, char *output_file);

// Define function prototypes for each version
namespace Ns_SSE2{ // SSE2 instruction set
    snpFuncType get_snps;
};     
namespace Ns_AVX{ // AVX instruction set
    snpFuncType get_snps;
};      
namespace Ns_AVX2{ // AVX2 instruction set
    snpFuncType get_snps;
};     
namespace Ns_AVX512{ // AVX512 instruction set
    snpFuncType get_snps;
};   

// Choose namespace name depending on which instruction set we compile for.
// ----------------------------------------------------------------------------
#if   INSTRSET >= 10                   // AVX512VL
#define SNP_DISPATCHED_NAMESPACE Ns_AVX512
#elif INSTRSET >= 8                    // AVX2
#define SNP_DISPATCHED_NAMESPACE Ns_AVX2
#elif INSTRSET >= 7                    // AVX
#define SNP_DISPATCHED_NAMESPACE Ns_AVX
#elif INSTRSET == 2
#define SNP_DISPATCHED_NAMESPACE Ns_SSE2   // SSE2
#else
#error Unsupported instruction set
#endif
// ----------------------------------------------------------------------------

namespace SNP_DISPATCHED_NAMESPACE {
    class SnpSite {
    private:
        string inputfile;
        int seq_length, num_of_snps;
        file_handler::FileHandler fh;
        int *snps_location;
        vector<Vec64c> refvecs;
    public:
        SnpSite(char* _inputfile);

        int is_unknown(char base);
        void detect_snps();
        void print_result(char* filename);
        void clean();
    };

    int get_snps(char *input_file, char *output_file);
}
