#include <zlib.h>
#include <string>

using namespace std;

typedef class _file_handler
{
private:
    string seq;
    gzFile file;
public:
    int open(string filename);
    void read_next_seq();
} file_handler;

int _file_handler::open(string filename)
{
    file = gzopen(filename.c_str(), "r");
}

void _file_handler::read_next_seq() {
    // TODO: implement me
}
