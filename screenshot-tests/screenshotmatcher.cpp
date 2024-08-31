#include <sstream>
#include <iostream>
#include <unistd.h>
#include "pyro/image.h"
#include "test-settings.h"

bool ImageMatch::match(std::filesystem::path const &filename) const
{
    if (access(this->actual_file.c_str(), F_OK) == -1)
    {
        FAIL_CHECK("Test target does not exist");
        return false;
    }

    if (access(filename.c_str(), F_OK) == -1)
    {
        FAIL_CHECK("Test did not produce a file");
        return false;
    }

    Pyro::Image *actual = Pyro::Image::load(this->actual_file);
    Pyro::Image *input = Pyro::Image::load(filename);

    // Check if same size
    if ((actual->width() != input->width()) || (actual->height() != input->height()))
    {
        delete (actual);
        delete (input);
        FAIL_CHECK("Screenshots are not the same size");
        return false;
    }

    // Check if same format
    if (actual->channels() != input->channels())
    {
        delete (actual);
        delete (input);
        FAIL_CHECK("Screenshots does not have the same channels");
        return false;
    }

    // Check all pixels
    const uint8_t *actual_pixels = actual->load_bytes();
    uint8_t *input_pixels = input->load_bytes();
    bool result = true;

    const unsigned int total_bytes = input->width() * input->height() * input->channels();

    unsigned int absolute_difference = 0;
    unsigned int pixel_difference = 0;

    for (unsigned int i = 0; i < input->width() * input->height() * input->channels(); i++)
    {
        if (actual_pixels[i] != input_pixels[i])
        {
            absolute_difference += abs(actual_pixels[i] - input_pixels[i]);
            pixel_difference += 1;
        }
    }

    delete (actual);
    delete (input);

    float deviance = absolute_difference / float(total_bytes * 255);

    if (deviance > this->tolerance)
    {
        result = false;
        FAIL_CHECK("Screenshots has a deviance of " << deviance << " and differ in " << pixel_difference << " pixels, with a total absolute difference of " << absolute_difference);
    }

    return result;
}

std::string ImageMatch::describe() const
{
    std::ostringstream ss;
    ss << "looks like \"" << actual_file << "\"";
    return ss.str();
}
