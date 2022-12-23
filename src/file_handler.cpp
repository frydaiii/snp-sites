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
    if (buffer_start >= buffer_end) {
        // if (eof) return -1;
        buffer_end = gzread(file, buffer, buffer_size);
        buffer_start = 0;
        if (buffer_end == 0) eof = true;
        // if (buffer_end == 0) return -1;
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
    /*
        TO DO (sort by priority):
        - return pointer instead of value.
        - use reverse to allocate mem, 2^n more each time.
        - use pair instead of tuple. 
        - optimize compiler.

        last solution: use char* :(
    */
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

pair<string, string> FileHandler::next_seq(int seq_length) {
    /*
        TO DO (sort by priority):
        - return pointer instead of value.
        - use reverse to allocate mem, 2^n more each time.
        - use pair instead of tuple. 
        - optimize compiler.

        last solution: use char* :(
    */
    string seq, sample_name = "";
    while (current_char != '>' && !eof) {
        current_char = next_char();
    }
    while (current_char = next_char(), current_char != '\n' && !eof) {
        sample_name += current_char;
    }
    // while (current_char = next_char(), current_char != '>' && !eof) {
    //     if (current_char == '\n') continue;
    //     seq += current_char;
    // }
    int i;
    while (true) {
        i = buffer_start; // stuck in here
        while (buffer[i] != '\n' && buffer[i] != '>' && i < buffer_end) i++;
        seq.append((char*)(buffer + i), i - buffer_start);
        if (buffer[i] == '>') break;
        if (buffer[i] == '\n') i++;
    }
    buffer_start = i;
    current_char = buffer[buffer_start];
    return {sample_name, seq};
}
