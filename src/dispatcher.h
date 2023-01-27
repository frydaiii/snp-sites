/*
This module contain dispatcher for speed-critical code.
*/
#ifndef DISPATCH_
#define DISPATCH_
#include "snp_sites.h"

// function prototypes for entry function and dispatcher, defined outside namespace
snpFuncType  snpfunc, snpfunc_dispatch;

// Dispatch function
void snpfunc_dispatch(char *input_file, char *output_file);


// Call the entry function through the function pointer.
// The first time this function is called, it goes through the dispatcher.
// The dispatcher will change the function pointer so that all subsequent
// calls go directly to the optimal version of the entry function
void snpfunc(char *input_file, char *output_file);
#endif