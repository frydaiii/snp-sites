#include "my_string.h"

#define kroundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

MyString::MyString() {
    len = 0; 
    cap = 1;
    s = (char*) malloc(cap * sizeof(char));
}

void MyString::append(char *_s, int _len) {
    /*
        Copy _len chars from _s to string.
    */
    if (len + _len > cap) {
        cap = len + _len;
        kroundup32(cap);
        s = (char*) realloc(s, cap);
    }
    memcpy(s + len, _s, _len);
    len += _len;
}

char* MyString::c_str() {
    /*
        Add null-terminated character and return string.
    */
    if (len + 1 > cap) {
        cap = len + 1;
        kroundup32(cap);
        s = (char*) realloc(s, cap);
    }
    s[len] = 0;
    return s;
}

int MyString::length() {
    return len;
}
