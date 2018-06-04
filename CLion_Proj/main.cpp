#include "bitmap_image.hpp"
#include "CLTest.cpp"

int main(int argc, char** argv) {
    bitmap_image imagetest(20, 20);

    const rgb_t myColour = make_colour(100, 255, 100);
    imagetest.set_pixel(5, 5, myColour);

    imagetest.save_image("test.bmp");

    testCL();
}