#ifndef SEQ_H_
#include <zlib.h>
#include <utility>
#include <ctype.h>
#include <string>

using namespace std;

#define SEQ_H_

#define BUFFER_SIZE 2048

namespace file_handler {
    typedef bool (*match_func)(int c, int delimiter);

    class FileHandler {
    private:
        gzFile file;
        char buffer[BUFFER_SIZE]; // Static allocation for better performance.
        int buffer_start, buffer_end;
        bool eof;
        void get_until(int delimiter, string *s);
    public:
        FileHandler();

        void open(const char* filename);
        void close();
        pair<string*, string*> next_sample();
        void assign_next_sample_to(string *name, string *seq);
        int next_char();
        bool is_eof();
    };
}
#endif