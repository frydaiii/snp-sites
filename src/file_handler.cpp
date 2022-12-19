#include "file_handler.h"

FileHandler::FileHandler(int _buffer_size) {
    buffer_start = 0;
    buffer_end = 0;
    eof = false;
    buffer_size = _buffer_size;
    buffer = (unsigned char*)malloc(buffer_size);
}

FileHandler::FileHandler() : FileHandler(2048) {}

void FileHandler::open(string filename) {
    file = gzopen(filename.c_str(), "r");
    eof = false;
    current_char = -1;
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

// string FileHandler::next_sample_name() {
//     string sample_name = "";
//     while (current_char != '>' && !eof) {current_char = next_char();}
//     while (current_char != '\n' && !eof) {
//         sample_name += current_char;
//         current_char = next_char();
//     }
//     return sample_name;
// }

tuple<string, string> FileHandler::next_seq() {
    // TODO: implement me
    string seq = "", sample_name = "";
    while (current_char != '>' && !eof) {
        current_char = next_char();
    }
    while (current_char = next_char(), current_char != '\n' && !eof) {
        sample_name += current_char;
    }
    while (current_char = next_char(), current_char != '>' && !eof) {
        if (current_char == '\n') {continue;}
        seq += current_char;
    }
    return {sample_name, seq};
}

bool FileHandler::is_eof() {
    return eof;
}