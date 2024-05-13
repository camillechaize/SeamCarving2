#pragma once
#include "utils.h"
#include <functional>

struct button
{
    IntPoint2 screenPosition = IntPoint2(0, 0);
    IntPoint2 scale = IntPoint2(0, 0);
    std::function<void()> onClickAction;
};

struct buttonsList
{
    int lastFreeIndex = 0;
    button listButtons[30];
};

void initWindow();

void updateWindow();

void drawButtons();

void addButton(button buttonToAdd);

void clearButtons();

void detectClick(IntPoint2 clickScreenPosition);

bool checkButtonClick(IntPoint2 clickScreenPosition);

// BUTTONS

void test();
