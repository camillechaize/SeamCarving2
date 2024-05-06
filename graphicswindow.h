#pragma once
#include "utils.h"
#include <functional>

struct button
{
    IntPoint2 screenPosition;
    IntPoint2 scale;
    std::function<void()> onClickAction;
};

struct buttonsList
{
    int lastFreeIndex = 0;
    button listButtons[30];
};

buttonsList onScreenButtons;

void addButton(button buttonToAdd);

void detectClick(IntPoint2 clickScreenPosition);

bool checkButtonClick(IntPoint2 clickScreenPosition);

// BUTTONS

void test();
