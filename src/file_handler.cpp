#include "file_handler.h"

FileHandler::FileHandler() {
    buffer_start = -1;
    buffer_end = -1;
    eof = false;
}

void FileHandler::open(char* filename) {
    file = gzopen(filename, "r");
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

// list of function to compare c and delimiter, need exactly 2 arguments.
bool match_space(int c, int delimter) {
    return isspace(c);
}
bool match_tab(int c, int delimter) {
    return isspace(c) && c != ' ';
}
bool match_newline(int c, int delimter) {
    return c == '\n';
}
bool match_char(int c, int delimter) {
    return c == delimter;
}
bool no_match(int c, int delimiter) {
    return false;
}
// end list.

void FileHandler::get_until(int delimiter, MyString *s) {
    /*
        Read till delimiter and append bytes read to s.
        When done cursor will be at the end of the line.
    */
    match_func match; // function to check if a char match delimiter
    switch (delimiter) {
        case SEP_SPACE:
            match = match_space;
            break;
        case SEP_TAB:
            match = match_tab;
            break;
        case SEP_LINE:
            match = match_newline;
            break;
        default:
            if (delimiter > SEP_MAX) match = match_char;
            else match = no_match;
    }

    // begin process
    int i = buffer_start;
    while (!match(buffer[i], delimiter)) {
        if (buffer_start >= buffer_end) {
            buffer_end = gzread(file, buffer, 2048);
            buffer_start = 0;
            i = 0;
            if (buffer_end == 0) {
                eof = true;
                break;
            }
        }
        while (!match(buffer[i], delimiter) && i < buffer_end) i++;
        s->append((char*)(buffer + buffer_start), i - buffer_start);
        buffer_start = i;
    }
}

/* Note: this function do not read quality score for QUAL file.*/
pair<MyString, MyString> FileHandler::next_sample() {
    MyString seq, name;
    int c;
    while (!eof && (c = next_char()) != '>' && c != '@') {} // read until meet sample name
    get_until(SEP_SPACE, &name); // get sample name
    while (!eof && (c = next_char()) != '>' && c != '@' && c != '+') {
        if (c == '\n') continue;
        get_until(SEP_LINE, &seq); // read sequence
    }
    buffer_start--; // step back to the end of sequence
    return {name, seq};
}
