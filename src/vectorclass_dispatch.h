// #include <stdio.h>
// #include "vectorclass/vectorclass.h"

// // Define function type
// // Change this to fit the entry function. Should not contain vector types:
// typedef float MyFuncType(float const []);

// // function prototypes for each version
// MyFuncType  myfunc_SSE2, myfunc_AVX, myfunc_AVX2, myfunc_AVX512;

// // function prototypes for common entry point and dispatcher
// MyFuncType  myfunc, myfunc_dispatch;

// // Define name of entry function depending on which instruction set we compile for
// #if   INSTRSET >= 10                   // AVX512VL
// #define FUNCNAME myfunc_AVX512
// #elif INSTRSET >= 8                    // AVX2
// #define FUNCNAME myfunc_AVX2
// #elif INSTRSET >= 7                    // AVX
// #define FUNCNAME myfunc_AVX
// #elif INSTRSET == 2
// #define FUNCNAME myfunc_SSE2           // SSE2
// #else
// #error Unsupported instruction set
// #endif
