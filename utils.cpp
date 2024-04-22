#include "utils.h"

void hslToRgb(float h, float s, float l, int &r, int &g, int &b)
{
    if (s == 0.0) {
        r = g = b = static_cast<int>(l * 255);
    } else {
        float temp1, temp2;
        float hue = h / 360.0;

        if (l < 0.5) {
            temp2 = l * (1.0 + s);
        } else {
            temp2 = (l + s) - (l * s);
        }

        temp1 = 2.0 * l - temp2;

        float tempR = hue + 1.0 / 3.0;
        float tempG = hue;
        float tempB = hue - 1.0 / 3.0;

        r = static_cast<int>(round(hueToRgbComponent(temp1, temp2, tempR) * 255));
        g = static_cast<int>(round(hueToRgbComponent(temp1, temp2, tempG) * 255));
        b = static_cast<int>(round(hueToRgbComponent(temp1, temp2, tempB) * 255));
    }
}

float hueToRgbComponent(float temp1, float temp2, float component)
{
    if (component < 0.0)
        component += 1.0;
    if (component > 1.0)
        component -= 1.0;

    if (6.0 * component < 1.0) {
        return temp1 + (temp2 - temp1) * 6.0 * component;
    } else if (2.0 * component < 1.0) {
        return temp2;
    } else if (3.0 * component < 2.0) {
        return temp1 + (temp2 - temp1) * ((2.0 / 3.0) - component) * 6.0;
    } else {
        return temp1;
    }
}

float distrib_1(float value, float min, float max)
{
    float norm_value = value / (max - min);
    float k = 6.f;
    return (1 - std::exp(-k * norm_value)) / (1 - std::exp(-k)) * (max - min);
}

float distrib_2(float value, float min, float max)
{
    float norm_value = value / (max - min);
    float k = 0.6f;
    return std::pow(2.f * (1.f - (1.f / (1.f + norm_value))), k) * (max - min);
}
