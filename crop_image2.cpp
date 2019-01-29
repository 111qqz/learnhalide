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
    Halide::Buffer<uint8_t> result(input.width(), input.height(), 3);
    /* Halide::BUffer.crop(int d,int min,int extent)
    　　d表示维度,对于一般的RGB头像，0代表width,1代表height,2代表channel
    　　min表示该维度上的base, extent表示该维度上基于base的offset.
    　　最后在该维度上得到的区间范围就是[min,min+extent)
    　　记得判断边界条件
    */
    
    result.crop(0,100,1000);
    result.crop(1,100,260);
    result.crop(2,1,1);

    brighter.realize(result);
    save_image(result, "croped2.png");



    printf("Success!\n");
    return 0;
}