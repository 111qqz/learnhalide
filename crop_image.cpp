#include "Halide.h"

// Include some support code for loading pngs.
#include "halide_image_io.h"
#include <iostream>
using namespace std;
using namespace Halide::Tools;

int main(int argc, char **argv) {

    // This program defines a single-stage imaging pipeline that
    // brightens an image.

    // First we'll load the input image we wish to brighten.
    Halide::Buffer<uint8_t> input = load_image("/data/renkuanze/png.png");
    cout<<input.width()<<" "<<input.height()<<" "<<input.channels()<<endl;

    // 手动crop
    Halide::Var x, y, c;
    Halide::Func brighter;
    brighter(x,y,c) = input(x,y,c);
    Halide::Buffer<uint8_t> result(input.width()/2, input.height()/2, 3);
    result.set_min(input.width()/4, input.height()/4);
    brighter.realize(result);
    save_image(result, "croped1.png");

    // Halide::Buffer<uint8_t> output =
    //     brighter.realize(input.width(), input.height(), input.channels());

    // // Save the output for inspection. It should look like a bright parrot.
    // save_image(output, "brighter2.png");

    // See figures/lesson_02_output.jpg for a small version of the output.

    printf("Success!\n");
    return 0;
}