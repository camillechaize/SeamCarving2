#pragma once
#include "Images_av.h"
#include "utils.h"
#include <functional>

struct button
{
    string text;
    IntPoint2 screenPosition = IntPoint2(0, 0);
    IntPoint2 scale = IntPoint2(0, 0);
    std::function<void()> onClickAction;
};

struct buttonsList
{
    int lastFreeIndex = 0;
    int totalHeight = 0;
    button listButtons[30];
};

void initWindow(string fileName);

void updateWindow(bool &flag);

void drawButtons();

void addButton(button buttonToAdd);

void clearButtons();

void detectClick(IntPoint2 clickScreenPosition);

bool checkButtonClick(IntPoint2 clickScreenPosition);

void clearImageArea();
void resizeImage(IntPoint2 clickScreenPosition);

// BUTTONS

void LoadImageButton();

void OpenOriginalButton();
void CalculateEnergyButton();
void ChangeEnergy_L1Button();
void ChangeEnergy_L2Button();
void ChangeEnergy_EntropyButton();

void Open_E_BWButton();
void Open_E_HeatButton();

void BrushEnterButton();
void BrushKeepButton();
void BrushRemoveButton();
void Brush5Button();
void Brush20Button();
void Brush50Button();
void Brush200Button();
void BrushExitButton();

void SeamCarving();

void Open_StepButton();

void ResizeEnterButton();
void ResizeExitButton();

void ExportImage();
