#include "file_handler.h"

FileHandler::FileHandler() {
    buffer_start = -1;
    buffer_end = -1;
    eof = false;
}

void FileHandler::open(string filename) {
    file = gzopen(filename.c_str(), "r");
    eof = false;
}

void FileHandler::close() {
    gzclose(file);
}

int FileHandler::next_char() {
    /* Read current character and increase cursor (buffer_start) by 1.*/
    if (buffer_start >= buffer_end) {
        buffer_end = gzread(file, buffer, 2048);
        buffer_start = -1;
        if (buffer_end == 0) eof = true;
    }
    return buffer[++buffer_start];
}

bool FileHandler::is_eof() {
    return eof;
}

void FileHandler::get_line_and_append_to(MyString *s) {
    /*
        Read to the end of current line and append bytes read to s.
        When done cursor will be at the end of the line.
    */

    int i = buffer_start;
    while (buffer[i] != '\n') {
        if (buffer_start >= buffer_end) {
            buffer_end = gzread(file, buffer, 2048);
            buffer_start = 0;
            i = 0;
            if (buffer_end == 0) {
                eof = true;
                break;
            }
        }
        while (buffer[i] != '\n' && i < buffer_end) i++;
        s->append((char*)(buffer + buffer_start), i - buffer_start);
        buffer_start = i;
    }
}

pair<MyString, MyString> FileHandler::next_seq() {
    MyString seq, sample_name;
    while (next_char() != '>' && !eof) {}
    get_line_and_append_to(&sample_name);
    while (next_char() != '>' && !eof) {
        get_line_and_append_to(&seq);
    }
    buffer_start--;
    return {sample_name, seq};
}
