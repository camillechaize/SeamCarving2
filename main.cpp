#include "Images_av.h"
#include "graphics.h"
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
    SC_image.LoadImage("img5.jpg");
    const int width = SC_image.width(),height = SC_image.height();
    Window Hidden_window = openWindow(width, height);
    Window Actual_window = openWindow(2*width + 1100, 2*height + 200);
    setActiveWindow(Actual_window);
    SC_image.ComputeEnergy();
    SC_image.OpenImage("energy","bw");
    milliSleep(5000);
    clearWindow();
    SC_image.OpenImage("original");
    Color* rgb = new Color[width*height];
    get_parts(Hidden_window,Actual_window,width,height,rgb);
    SC_image.Chooseparttoremove(rgb,width,height);
    clearWindow();
    SC_image.OpenImage("energy","bw");
    milliSleep(5000);
    SC_image.ComputeAllVerticalSeams();
    cout << rgb[0] << endl;
    for (int i = 0; i < 7200; ++i) {
        noRefreshBegin();
        clearWindow();
        SC_image.OpenImage("size", std::to_string(720 - (i % 720)));
        noRefreshEnd();
        milliSleep(10);
    }
    endGraphics();
    return 0;
}
