#include "graphics.h"

int keyboard() {
    Event e;
    do {
        getEvent(0,e);
        if(e.type==EVT_KEY_ON)
            return e.key;
    } while(e.type!=EVT_NONE);
    return 0;
}

void get_coeffs(double coeffs[2]){
    Im img;
    assert (load(img,srcPath("How_much_do_you_want_to_magnify_or_shrink_the_image"))); // Stop si l'image n'est pas chargee
    coeffs[0] = 1.0;
    int vc1,vc2;
    coeffs[1] = 1.0;
    int hc1,hc2;
    int const imgw = img.width();
    int const imgh = img.height();
    bool idx = 0;
    openWindow(imgw, imgh);
    display(img);
    int vertical_x[3] = {153,193,233};
    int y1[3] = {430,480,430};
    int y2[3] = {350,300,350};
    int horizontal_x[3] = {683,723,763};
    int key = 0;
    int first,second;
    vc1 = coeffs[0]/1;
    vc2 = (coeffs[0] - vc1)*10;
    hc1 = coeffs[1]/1;
    hc2 = (coeffs[1] - hc1)*10;
    drawPoly(vertical_x,y2,3,RED);
    drawPoly(vertical_x,y1,3,RED);
    drawPoly(horizontal_x,y2,3,RED);
    drawPoly(horizontal_x,y1,3,RED);
    drawString(170,400,to_string(vc1)+'.'+to_string(vc2),BLACK);
    drawString(700,400,to_string(hc1)+'.'+to_string(hc2),BLACK);
    while(true){
        vc1 = coeffs[0]/1;
        vc2 = (coeffs[0] - vc1)*10;
        hc1 = coeffs[1]/1;
        hc2 = (coeffs[1] - hc1)*10;
        key = keyboard();
        switch (key){
        case KEY_UP:
            if(coeffs[idx]<2.0){
                if (idx==0){
                    fillPoly(vertical_x,y2,3,RED);
                    drawString(170,400,to_string(vc1)+'.'+to_string(vc2),WHITE);
                }
                else{
                    fillPoly(horizontal_x,y2,3,RED);
                    drawString(700,400,to_string(hc1)+'.'+to_string(hc2),WHITE);
                }
                coeffs[idx] += 0.1;
                vc1 = coeffs[0]/1;
                vc2 = (coeffs[0] - vc1)*10;
                hc1 = coeffs[1]/1;
                hc2 = (coeffs[1] - hc1)*10;
                if (idx==0){
                    drawString(170,400,to_string(vc1)+'.'+to_string(vc2),RED);
                }
                else{
                    drawString(700,400,to_string(hc1)+'.'+to_string(hc2),RED);
                }
                milliSleep(50);
                fillPoly(vertical_x,y2,3,WHITE);
                fillPoly(horizontal_x,y2,3,WHITE);
                drawPoly(vertical_x,y2,3,RED);
                drawPoly(horizontal_x,y2,3,RED);
            }
            break;
        case KEY_RIGHT:
            idx = 1;
            drawString(170,400,to_string(vc1)+'.'+to_string(vc2),WHITE);
            drawString(170,400,to_string(vc1)+'.'+to_string(vc2),BLACK);
            drawString(700,400,to_string(hc1)+'.'+to_string(hc2),WHITE);
            drawString(700,400,to_string(hc1)+'.'+to_string(hc2),RED);
            break;
        case KEY_LEFT:
            idx = 0;
            drawString(700,400,to_string(hc1)+'.'+to_string(hc2),WHITE);
            drawString(700,400,to_string(hc1)+'.'+to_string(hc2),BLACK);
            drawString(170,400,to_string(vc1)+'.'+to_string(vc2),WHITE);
            drawString(170,400,to_string(vc1)+'.'+to_string(vc2),RED);
            break;
        case KEY_DOWN:
            if(coeffs[idx]>=0.2){
                if (idx==0){
                    fillPoly(vertical_x,y1,3,RED);
                    drawString(170,400,to_string(vc1)+'.'+to_string(vc2),WHITE);
                }
                else{
                    fillPoly(horizontal_x,y1,3,RED);
                    drawString(700,400,to_string(hc1)+'.'+to_string(hc2),WHITE);
                }
                coeffs[idx] -= 0.1;
                vc1 = coeffs[0]/1;
                vc2 = (coeffs[0] - vc1)*10;
                hc1 = coeffs[1]/1;
                hc2 = (coeffs[1] - hc1)*10;
                if (idx==0){
                    drawString(170,400,to_string(vc1)+'.'+to_string(vc2),RED);
                }
                else{
                    drawString(700,400,to_string(hc1)+'.'+to_string(hc2),RED);
                }
                milliSleep(50);
                fillPoly(vertical_x,y1,3,WHITE);
                fillPoly(horizontal_x,y1,3,WHITE);
                drawPoly(vertical_x,y1,3,RED);
                drawPoly(horizontal_x,y1,3,RED);
            }
            break;
        }
    }
    endGraphics();
}
