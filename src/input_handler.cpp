#include "input_handler.h"

InputHandler::InputHandler() {
}

void InputHandler::open(const char* filename) {
    this->instream.reset();
    std::ifstream file(filename);
    this->instream.push(boost::iostreams::gzip_decompressor());
    this->instream.push(boost::iostreams::file_source(filename));
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
    } // read until meet sample name
    if (!getline(this->instream, *name)) return false;
    string line;
    while (getline(this->instream, line)) {
        seq->append(line);
        c = this->instream.peek();
        if (c == '>' || c == '@' || c == '+') break;
    }
    return true;
}
