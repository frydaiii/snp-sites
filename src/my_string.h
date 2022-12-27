#include <stdlib.h>
#include <string.h>

class MyString {
private:
    int len, cap;
    char *s;
public:
    MyString();

    void append(char *_s, int _len);
    char* c_str();
    int length();
    void clean();
};
