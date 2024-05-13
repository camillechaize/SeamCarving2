#include <Imagine/Images.h>
#include <Imagine/Graphics.h>
#include <iostream>

using namespace Imagine;

typedef Image<byte> Im;
using namespace std;


int keyboard();

void get_parts(Window hidden_window, Window actual_window, int width, int height, Color* rgb);

void interface(int width, int height, int list_of_choices[]);

