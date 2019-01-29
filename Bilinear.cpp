#include "Halide.h"

// Include some support code for loading pngs.
#include "halide_image_io.h"
#include <iostream>
using namespace std;
using namespace Halide;
using namespace Halide::Tools;

int main(int argc, char **argv) {

    
    // Halide::Buffer<uint8_t> input = load_image("/data/renkuanze/png.png");
    // cout<<input.width()<<" "<<input.height()<<" "<<input.channels()<<endl;
    Func clamped("clamped"),clamped2("clamped2");
    // // 将图片密铺，避免判断边界情况。
    // clamped2 = BoundaryConditions::repeat_edge(input);

    // // 手动crop
    Halide::Var x("x"), y("y"), c("c");
    // Halide::Func brighter("brighter");
    // brighter(x,y,c) = clamped2(x,y,c);
    // Halide::Buffer<uint8_t> croped(input.width(), input.height(), 3);
    // /* Halide::BUffer.crop(int d,int min,int extent)
    // 　　d表示维度,对于一般的RGB头像，0代表width,1代表height,2代表channel
    // 　　min表示该维度上的base, extent表示该维度上基于base的offset.
    // 　　最后在该维度上得到的区间范围就是[min,min+extent)
    // */
    
    // croped.crop(0,0,620);
    // croped.crop(1,0,230);
    // cout<<croped.width() <<" "<<croped.height() <<" "<<croped.channels()<<endl;
    // brighter.realize(croped);
    // save_image(croped, "croped.png");

    Halide::Buffer<uint8_t> croped = load_image("croped.png");


    Func result("result");
    clamped = BoundaryConditions::repeat_edge(croped);
    result(x,y,c) = clamped(x,y,c);
 
    auto srcWidth = croped.width();
    auto srcHeight = croped.height();
    auto dstWidth = srcWidth * 8;
    auto dstHeight = srcHeight * 8;
    Var ox{"ox"}, oy{"oy"};
    Func oxf("oxf"), oyf("oyf"), ix("ix"), iy("iy"), ix0("ix0"), iy0("iy0"), ix1("ix1"), iy1("iy1"), wx("wx"), wy("wy");
    oxf(ox, oy) = (ox + .5f) / dstWidth;
    oyf(ox, oy) = (oy + .5f) / dstHeight;
    ix(ox, oy) = oxf(ox, oy) * srcWidth - .5f;
    iy(ox, oy) = oyf(ox, oy) * srcHeight - .5f;

     // coords of 4 pixels involved
    ix0(ox, oy) = cast<int>(floor(ix(ox, oy)));
    iy0(ox, oy) = cast<int>(floor(iy(ox, oy)));
    ix1(ox, oy) = ix0(ox, oy) + 1;//cast<int>(ceil(ix(ox, oy)));
    iy1(ox, oy) = iy0(ox, oy) + 1;//cast<int>(ceil(iy(ox, oy)));
    // interpolation weights
    wx(ox, oy) = ix(ox, oy) - ix0(ox, oy);
    wy(ox, oy) = iy(ox, oy) - iy0(ox, oy);

    Func p00("p00"), p01("p01"), p10("p10"), p11("p11");
    Func vx0("vx0"), vx1("vx1"), dst("dst");
    p00(ox, oy, c) = clamped(ix0(ox, oy), iy0(ox, oy), c);
    p01(ox, oy, c) = clamped(ix1(ox, oy), iy0(ox, oy), c);
    p10(ox, oy, c) = clamped(ix0(ox, oy), iy1(ox, oy), c);
    p11(ox, oy, c) = clamped(ix1(ox, oy), iy1(ox, oy), c);

    vx0(ox, oy, c) = lerp(p00(ox, oy, c), p01(ox, oy, c), wx(ox, oy));
    vx1(ox, oy, c) = lerp(p10(ox, oy, c), p11(ox, oy, c), wx(ox, oy));
    dst(ox, oy, c) = lerp(vx0(ox, oy, c), vx1(ox, oy, c), wy(ox, oy));
    Halide::Buffer<uint8_t> output(dstWidth, dstHeight, 3);
    // output(ox, oy, c) = cast<uint8_t>(dst(ox, oy, c)*0);
    Func out("out");
    out(ox,oy,c) = cast<uint8_t>(dst(ox, oy, c));
    out.realize(output); //把out函数实现在output buffer上
    save_image(output, "Bilinear7.png");


    printf("Success!\n");
    return 0;
}