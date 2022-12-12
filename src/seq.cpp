#include "seq.h"

FileHandler::FileHandler() {
    buffer_start = 0;
    buffer_end = 0;
    eof = false;
    buffer_size = 2048;
}

FileHandler::FileHandler(int _buffer_size) {
    buffer_start = 0;
    buffer_end = 0;
    eof = false;
    buffer_size = _buffer_size;
}

void FileHandler::open(string filename) {
    file = gzopen(filename.c_str(), "r");
}

int FileHandler::next_char() {
    if (buffer_end <= buffer_start) {
        buffer = (unsigned char*)malloc(buffer_size);
        buffer_end = gzread(file, buffer, buffer_size);
        buffer_start = 0;
    }
    return buffer[buffer_start++];
}

string FileHandler::next_seq() {
    // TODO: implement me
    seq = "";
    int c;
    while (c = next_char() != '>') {}
    while (c = next_char() != '\n') {}
    while (c = next_char() != '\n') {
        seq += c;
    }
    return seq;
}
