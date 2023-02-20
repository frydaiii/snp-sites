#include "input_handler.h"

InputHandler::InputHandler() {
    this->buffer_start = -1;
    this->buffer_end = -1;
    this->eof = false;
}

void InputHandler::open(const char* filename) {
    this->file = gzopen(filename, "r");
    this->eof = false;

    this->instream.reset();
    std::ifstream file(filename);
    this->instream.push(boost::iostreams::gzip_decompressor());
    this->instream.push(boost::iostreams::file_source(filename));
    // this->instream.seekg(-1);
    // this->instream.;
    // streampos begin = this->instream.tellg();
    // cout << begin << endl;
    // boost::iostreams::filtering_streambuf<boost::iostreams::input> inbuf;
    // inbuf.push(boost::iostreams::gzip_decompressor());
    // inbuf.push(file);

    // convert streambuf to ifstream
    // this->instream.basic_istream<char>::rdbuf(&inbuf);
    // this->instream.open(filename);
}

void InputHandler::close() {
    gzclose(file);
}

/* Read current character and increase cursor (buffer_start) by 1.*/
int InputHandler::next_char() {
    if (this->buffer_start >= this->buffer_end) {
        this->buffer_end = gzread(file, buffer, BUFFER_SIZE);
        this->buffer_start = -1;
        if (this->buffer_end == 0) this->eof = true;
    }
    return this->buffer[++this->buffer_start];
}

bool InputHandler::is_eof() {
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
void InputHandler::get_until(int delimiter, string *s) {
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
bool InputHandler::assign_next_sample_to(string *name, string *seq) {
    name->erase();
    seq->erase();
    char c;
    while (c != '>' && c != '@' && !this->instream.eof()) {
        c = this->instream.get();
        // if (c == '>' || c == '@' || c == EOF) break;
        // c = get(this->instream);
    } // read until meet sample name
    // get_until(SEP_SPACE, name); // get sample name
    if (!getline(this->instream, *name)) return false;
    while (getline(this->instream, *seq)) {
        c = this->instream.peek();
        if (c == '>' || c == '@' || c == '+') break;
        // if (c == '\n') continue;
        // get_until(SEP_LINE, seq); // read sequence
        // this->instream.getline(seq->data(), seq->capacity());
    }
    // buffer_start--; // step back to the end of sequence
    return true;
}

void InputHandler::back2begin() {
    this->instream.clear();
    this->instream.seekg(0, std::ios::beg);
}