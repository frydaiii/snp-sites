#include "seq.h"

FileHandler::FileHandler() {
    // buffer_start = 0;
    // buffer_end = 0;
    // eof = false;
    // buffer_size = 2048;
    // buffer = (unsigned char*)malloc(buffer_size);
    FileHandler(2048);
}

FileHandler::FileHandler(int _buffer_size) {
    buffer_start = 0;
    buffer_end = 0;
    eof = false;
    buffer_size = _buffer_size;
    buffer = (unsigned char*)malloc(buffer_size);
}

void FileHandler::open(string filename) {
    file = gzopen(filename.c_str(), "r");
}

void FileHandler::close() {
    gzclose(file);
}

int FileHandler::next_char() {
    if (buffer_end <= buffer_start) {
        buffer_end = gzread(file, buffer, buffer_size);
        buffer_start = 0;
    }
    if (buffer_end < buffer_size && buffer_start >= buffer_end) {
        eof = true;
    }
    return buffer[buffer_start++];
}

string FileHandler::next_sample_name() {
    string sample_name = "";
    int c;
    while (c = next_char(), c != '>' && !eof) {}
    while (c = next_char(), c != '\n' && !eof) {
        sample_name += c;
    }
    return sample_name;
}

string FileHandler::next_seq() {
    // TODO: implement me
    string seq = "";
    int c;
    while (c = next_char(), c != '>' && !eof) {}
    while (c = next_char(), c != '\n' && !eof) {}
    while (c = next_char(), c != '\n' && !eof) {
        seq += c;
    }
    return seq;
}

bool FileHandler::is_eof() {
    return eof;
}