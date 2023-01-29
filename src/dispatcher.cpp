#include "dispatcher.h"

snpFuncType *snpfunc_pointer = &snpfunc_dispatch;                // function pointer

// Dispatch function
int snpfunc_dispatch(char *input_file, char *output_file) {
    int iset = instrset_detect();                              // Detect supported instruction set
    // Choose which version of the entry function we want to point to:
    if      (iset >= 10) snpfunc_pointer = &Ns_AVX512::get_snps;  // AVX512 version
    else if (iset >=  8) snpfunc_pointer = &Ns_AVX2::get_snps;    // AVX2 version
    else if (iset >=  5) snpfunc_pointer = &Ns_AVX::get_snps;     // AVX version
    else if (iset >=  2) snpfunc_pointer = &Ns_SSE2::get_snps;    // SSE2 version
    else {
        // Error: lowest instruction set not supported.
        // Put any appropriate error handler here
        fprintf(stderr, "\nError: Instruction set SSE2 not supported on this computer");
    }
    // continue in the dispatched version of the entry function
    return (*snpfunc_pointer)(input_file, output_file);
}

// Call the entry function through the function pointer.
// The first time this function is called, it goes through the dispatcher.
// The dispatcher will change the function pointer so that all subsequent
// calls go directly to the optimal version of the entry function
int snpfunc(char *input_file, char *output_file) {
    return (*snpfunc_pointer)(input_file, output_file);                 // go to dispatched version
}