### calloc vs malloc
- malloc return memory as it is from an operating system.
- calloc gets memory from the kernel or operating system and its initializes with its zero and return.
```
Assuming the total amount of memory being initialized in your two examples is the same, allocating the memory with calloc() might be faster than allocating the memory with malloc() and then zeroing them out in a separate step, especially if in the malloc() case you zero the elements individually by iterating over them in a loop. A malloc() followed by a memset() will likely be about as fast as calloc().
```
> [Why malloc+memset is slower than calloc?](https://stackoverflow.com/questions/2688466/why-mallocmemset-is-slower-than-calloc)

