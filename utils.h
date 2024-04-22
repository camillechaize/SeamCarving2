#pragma once
#include <Imagine/Graphics.h>
#include <Imagine/Images.h>
#include <cmath>

using namespace Imagine;
using namespace std;

void hslToRgb(float h, float s, float l, int &r, int &g, int &b);

float hueToRgbComponent(float temp1, float temp2, float component);

float distrib_1(float value, float min = 0.f, float max = 1.f);

float distrib_2(float value, float min = 0.f, float max = 1.f);
