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

void get_parts(int width, int height, Color* &rgb){
    int* list_of_points_x = new int[1000];
    int* list_of_points_y = new int[1000];
    int idx = 0;
    Event get_mouse;
        idx = 0;
        Color col;
        if (anyClick()== 1){
            col = RED;
        }
        else {
            col = GREEN;
        }
        while(true){
            getEvent(-1,get_mouse);
            if (get_mouse.type == EVT_MOTION ){
                list_of_points_x[idx] = get_mouse.pix.x();
                list_of_points_y[idx] = get_mouse.pix.y();
                idx++;
                if (idx >1){
                    drawLine(list_of_points_x[idx-1],list_of_points_y[idx-1],list_of_points_x[idx-2],list_of_points_y[idx-2],col);
                }
            }
            if((abs(list_of_points_x[idx-1]- list_of_points_x[0])<5)&&(abs(list_of_points_y[idx-1]- list_of_points_y[0])<5)&&(idx>10)){
                break;
            }
        }
        clearWindow();
        fillPoly(list_of_points_x,list_of_points_y,idx,col);
    captureRectangle(0,0,rgb,width,height);
    clearWindow();
}

void interface(int width, int height ,int list_of_choices[]){
    Color grey(125,125,125);
    fillRect(2*width+100,height/3,400,height/6,grey);
    fillRect(2*width+100,2*height/3,400,height/6,grey);
    fillRect(2*width+100,height,400,height/6,grey);
    drawRect(2*width+100,height/3,400,height/6,BLACK,3);
    drawRect(2*width+100,2*height/3,400,height/6,BLACK,3);
    drawRect(2*width+100,height,400,height/6,BLACK,3);
    fillRect(2*width+600,height/3,400,height/6,grey);
    fillRect(2*width+600,2*height/3,400,height/6,grey);
    fillRect(2*width+600,height,400,height/6,grey);
    drawRect(2*width+600,height/3,400,height/6,BLACK,3);
    drawRect(2*width+600,2*height/3,400,height/6,BLACK,3);
    drawRect(2*width+600,height,400,height/6,BLACK,3);
    drawString(2*width+100,height/3+height/12,"vertical coefficient : 1.0",BLACK);
    drawString(2*width+600,height/3+height/12,"horizontal coefficient : 1.0",BLACK);
    drawString(2*width+100,2*height/3+height/12,"energy : entropy",BLACK);
    drawString(2*width+600,2*height/3+height/12,"edit image",BLACK);
    drawString(2*width+100,height+height/12,"display energy",BLACK);
    drawString(2*width+600,height+height/12,"generate image",BLACK);
    drawString(0,2*height+50,"select the zone to keep in the picture with a left click",RED);
    drawString(0,2*height+150,"select the zone to delete in the picture with a right click",GREEN);
    double coeffs[2];
    coeffs[0] = 1.0;
    int vc1,vc2;
    coeffs[1] = 1.0;
    int hc1,hc2;
    bool idx = 0;
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
