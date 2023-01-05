// #include "vectorclass_dispatch.h"

// // This is the dispatched function that is compiled in multiple versions with different names.
// // Make sure this function is static to prevent clash with other versions having the same name.
// // The function cannot be member of a class.
// static float sum (float const f[]) {
//     // This example adds 16 floats
//     Vec16f a;                          // vector of 16 floats
//     a.load(f);                         // load array into vector
//     return horizontal_add(a);          // return sum of 16 elements
// }

// // -----------------------------------------------------------------------------
// //                       Entry function
// // -----------------------------------------------------------------------------
// // This is the entry function that is accessed through the dispatcher.
// // This serves as the interface between the common code and the dispatched code.
// // The entry function cannot be member of a class.
// // The entry function must use arrays rather than vectors for input and output.
// float FUNCNAME (float const f[]) {
//     return sum(f);
// }

// // ---------------------------------------------------------------------------------
// //                       Dispacther
// // ---------------------------------------------------------------------------------
// // This function pointer initially points to the dispatcher.
// // After the first call, it points to the selected version of the entry function
// MyFuncType * myfunc_pointer = &myfunc_dispatch;            // function pointer

// // Dispatcher
// float myfunc_dispatch(float const f[]) {
//     int iset = instrset_detect();                          // Detect supported instruction set
//     // Choose which version of the entry function we want to point to:
//     if      (iset >= 10) myfunc_pointer = &myfunc_AVX512;  // AVX512 version
//     else if (iset >=  8) myfunc_pointer = &myfunc_AVX2;    // AVX2 version
//     else if (iset >=  7) myfunc_pointer = &myfunc_AVX;     // AVX version
//     else if (iset >=  2) myfunc_pointer = &myfunc_SSE2;    // SSE2 version
//     else {
//         // Error: lowest instruction set not supported.
//         // Put any appropriate error handler here
//         fprintf(stderr, "\nError: Instruction set SSE2 not supported on this computer");
//         return 0.f;
//     }
//     // continue in dispatched version of the function
//     return (*myfunc_pointer)(f);
// }


// // Call the entry function through the function pointer.
// // The first time this function is called, it goes through the dispatcher.
// // The dispatcher will change the function pointer so that all subsequent
// // calls go directly to the optimal version of the entry function
// inline float myfunc(float const f[]) {
//     return (*myfunc_pointer)(f);                 // go to dispatched version
// }
