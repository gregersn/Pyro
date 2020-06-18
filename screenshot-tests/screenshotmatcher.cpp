#include <sstream>
#include <iostream>
#include <unistd.h>
#include "pyro/pyroimage.h"
#include "test-settings.h"


bool ImageMatch::match(std::string const& filename) const {
    if(access(this->actual_file.c_str(), F_OK) == -1) {
        FAIL_CHECK("Test target does not exist");
        return false;
    }

    if(access(filename.c_str(), F_OK) == -1) {
        FAIL_CHECK("Test did not produce a file");
        return false;
    }

    Pyro::Image *actual = Pyro::Image::load(this->actual_file);
    Pyro::Image *input = Pyro::Image::load(filename);

    // Check if same size
    if((actual->width() != input->width()) || (actual->height() != input->height())) {
        delete(actual);
        delete(input);
        FAIL_CHECK("Screenshots are not the same size");
        return false;
    }

    // Check if same format
    if(actual->channels() != input->channels()) {
        delete(actual);
        delete(input);
        FAIL_CHECK("Screenshots does not have the same channels");
        return false;
    }

    // Check all pixels
    // TODO: Add tolerance(?)
    uint8_t *actual_pixels = actual->load_bytes();
    uint8_t *input_pixels = input->load_bytes();
    bool result = true;

    for(unsigned int i = 0; i < input->width() * input->height() * input->channels(); i++) {
        if(actual_pixels[i] != input_pixels[i]) {
            // TODO: Create a diff
            result = false;
            break;
        }
    }

    delete(actual);
    delete(input);

    return result;
}

std::string ImageMatch::describe() const {
    std::ostringstream ss;
    ss << "looks like \"" << actual_file << "\"";
    return ss.str();
}

