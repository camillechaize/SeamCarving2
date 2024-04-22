#include "Images_av.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <iostream>

int window_w = 1200, window_h = 700;

Img Image_energy(Img I)
{
    return I;
}

int main()
{
    Images_av SC_image;
    openWindow(window_w, window_h);
    SC_image.LoadImage("img5.jpg");
    SC_image.ComputeEnergy();
    SC_image.ComputeAllVerticalSeams();
    SC_image.OpenImage("seams");
    milliSleep(2000);
    for (int i = 0; i < 3600; ++i) {
        noRefreshBegin();
        clearWindow();
        SC_image.OpenImage("size", std::to_string(360 - (i % 360)));
        noRefreshEnd();
        milliSleep(10);
    }
    endGraphics();
    return 0;
}
