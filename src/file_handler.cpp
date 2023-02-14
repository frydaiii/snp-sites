#include "file_handler.h"

file_handler::FileHandler::FileHandler() {
    this->buffer_start = -1;
    this->buffer_end = -1;
    this->eof = false;
}

void file_handler::FileHandler::open(const char* filename) {
    this->file = gzopen(filename, "r");
    this->eof = false;
}

void file_handler::FileHandler::close() {
    gzclose(file);
}

/* Read current character and increase cursor (buffer_start) by 1.*/
int file_handler::FileHandler::next_char() {
    if (this->buffer_start >= this->buffer_end) {
        this->buffer_end = gzread(file, buffer, BUFFER_SIZE);
        this->buffer_start = -1;
        if (this->buffer_end == 0) this->eof = true;
    }
    return this->buffer[++this->buffer_start];
}

bool file_handler::FileHandler::is_eof() {
    return this->eof;
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

int growthCap(const int& old_cap, const int& new_len) {
    int new_cap = old_cap, double_cap = 2 * old_cap;
    if (new_len > double_cap) {
        new_cap = new_len;
    } else {
        int const threshold = 256;
        if (old_cap < threshold) {
            new_cap = double_cap;
        } else {
            while (0 < new_cap && new_cap < new_len) {
                new_cap += (new_cap + 3*threshold) / 4;
            }
            if (new_cap <= 0) {
				new_cap = new_len;
			}
        }
    }
    return new_cap;
}

/*
    Read till delimiter and append bytes read to s.
    When done cursor will be at the end of the line.
*/
void file_handler::FileHandler::get_until(int delimiter, string *s) {
    file_handler::match_func match; // function to check if a char match delimiter
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
    int i = this->buffer_start;
    while (!match(buffer[i], delimiter)) {
        if (this->buffer_start >= this->buffer_end) {
            this->buffer_end = gzread(this->file, this->buffer, BUFFER_SIZE);
            this->buffer_start = 0;
            i = 0;
            if (this->buffer_end == 0) {
                this->eof = true;
                break;
            }
        }
        while (!match(this->buffer[i], delimiter) && i < this->buffer_end) i++;
        if (s->capacity() < (i - this->buffer_start) + 1) {
            int newcap = s->capacity() + i - this->buffer_start;
            newcap = growthCap(s->capacity(), newcap);
            s->reserve(newcap);
        }
        s->append((char*)(this->buffer + this->buffer_start), i - this->buffer_start);
        this->buffer_start = i;
    }
}

/* 
    Get next sample name and sequence, assign it to *name and *seq.
    Note: this function do not read quality score for QUAL file.
*/
void file_handler::FileHandler::assign_next_sample_to(string *name, string *seq) {
    name->erase();
    seq->erase();
    int c;
    while (!eof && (c = next_char()) != '>' && c != '@') {} // read until meet sample name
    get_until(SEP_SPACE, name); // get sample name
    while (!eof && (c = next_char()) != '>' && c != '@' && c != '+') {
        if (c == '\n') continue;
        get_until(SEP_LINE, seq); // read sequence
    }
    buffer_start--; // step back to the end of sequence
}
