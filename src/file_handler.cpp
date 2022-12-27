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

#define SEP_SPACE 0 // isspace(): \t, \n, \v, \f, \r
#define SEP_TAB   1 // isspace() && !' '
#define SEP_LINE  2 // line separator: "\n" (Unix) or "\r\n" (Windows)
#define SEP_MAX   2

bool FileHandler::match_delimiter(int delimiter, int c) {
    /*
        Check if c match delimiter.
    */
    switch (delimiter) {
        case SEP_SPACE:
            return isspace(c);
        case SEP_TAB:
            return isspace(c) && c != ' ';
        case SEP_LINE:
            return c == '\n';
        default:
            if (delimiter > SEP_MAX) return c == delimiter;
            else return false;
    }
}

void FileHandler::get_until(int delimiter, MyString *s) {
    /*
        Read till delimiter and append bytes read to s.
        When done cursor will be at the end of the line.
    */

    int i = buffer_start;
    while (!match_delimiter(delimiter, buffer[i])) {
        if (buffer_start >= buffer_end) {
            buffer_end = gzread(file, buffer, 2048);
            buffer_start = 0;
            i = 0;
            if (buffer_end == 0) {
                eof = true;
                break;
            }
        }
        while (!match_delimiter(delimiter, buffer[i]) && i < buffer_end) i++;
        s->append((char*)(buffer + buffer_start), i - buffer_start);
        buffer_start = i;
    }
}

pair<MyString, MyString> FileHandler::next_sample() {
    MyString seq, name;
    int c;
    while (!eof && (c = next_char()) != '>' && c != '@') {} // read until meet sample name
    get_until(SEP_SPACE, &name); // get sample name
    while (!eof && (c = next_char()) != '>' && c != '@' && c != '+') {
        get_until(SEP_LINE, &seq); // read sequence
    }
    buffer_start--; // step back to the end of sequence
    return {name, seq};
}
