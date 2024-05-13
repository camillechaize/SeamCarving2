
#include "Images_av.h"

#include "graphicswindow.h"
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <iostream>

int window_w = 1200, window_h = 900;

int main()
{
    Images_av SC_image;
    openWindow(window_w, window_h);

    string fileName = "img5.jpg";
    //    string fileNameBrush = "img5_brush.png";

    initWindow();

    SC_image.LoadImage(fileName);
    //    SC_image.LoadImage(fileNameBrush, "layer");
    SC_image.ComputeEnergy("gradient");
    //    SC_image.ApplyBrushEnergy();
    SC_image.ComputeAllVerticalSeams();
    SC_image.OpenImage("energy", "heat");

    int x, y;

    // Get point mouse
    getMouse(x, y);

    detectClick(IntPoint2(x, y));

    for (int i = 0; i < 7200; ++i) {
        noRefreshBegin();
        clearWindow();
        SC_image.OpenImage("size", std::to_string(360));
        noRefreshEnd();
        milliSleep(10);
    }

    endGraphics();
    return 0;
}
