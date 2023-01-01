#include "file_handler.h"

#define kroundup32(x) (--(x), (x)|=(x)>>1, (x)|=(x)>>2, (x)|=(x)>>4, (x)|=(x)>>8, (x)|=(x)>>16, ++(x))

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

void FileHandler::get_until(int delimiter, string *s) {
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
        if (s->capacity() - (i - buffer_start) < 1) {
            int newcap = s->capacity() + i - buffer_start;
            kroundup32(newcap);
            // newcap = growthCap(s->capacity(), newcap);
            s->reserve(newcap);
        }
        s->append((char*)(buffer + buffer_start), i - buffer_start);
        buffer_start = i;
    }
}

/* Note: this function do not read quality score for QUAL file.*/
pair<string*, string*> FileHandler::next_sample() {
    string *seq = new string, *name = new string;
    int c;
    while (!eof && (c = next_char()) != '>' && c != '@') {} // read until meet sample name
    get_until(SEP_SPACE, name); // get sample name
    while (!eof && (c = next_char()) != '>' && c != '@' && c != '+') {
        if (c == '\n') continue;
        get_until(SEP_LINE, seq); // read sequence
    }
    buffer_start--; // step back to the end of sequence
    return {name, seq};
}

/* Note: this function do not read quality score for QUAL file.*/
void FileHandler::assign_next_sample_to(string *name, string *seq) {
    /* Get next sample name and sequence, assign it to *name and *seq.*/
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
