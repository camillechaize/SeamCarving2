#include "graphicswindow.h"

buttonsList onScreenButtons;
string state = "init"; // init / NI: no image loaded /

void addButton(button buttonToAdd)
{
    // Add button to list of on screen buttons
    onScreenButtons.listButtons[onScreenButtons.lastFreeIndex] = buttonToAdd;
    onScreenButtons.lastFreeIndex += 1;
}

void clearButtons()
{
    onScreenButtons.lastFreeIndex = 0;
}

void drawButtons()
{
    for (int i = 0; i < onScreenButtons.lastFreeIndex; ++i) {
        drawRect(onScreenButtons.listButtons[i].screenPosition,
                 onScreenButtons.listButtons[i].scale.x(),
                 onScreenButtons.listButtons[i].scale.y(),
                 GREEN);
    }
}

void detectClick(IntPoint2 clickScreenPosition)
{
    if (!checkButtonClick(clickScreenPosition)) {
        std::cout << "no button clicked";
    }
}

bool checkButtonClick(IntPoint2 clickScreenPosition)
{
    for (int i = 0; i < onScreenButtons.lastFreeIndex; ++i) {
        if (clickScreenPosition.x() < onScreenButtons.listButtons[i].screenPosition.x()
                                          + onScreenButtons.listButtons[i].scale.x()
            && clickScreenPosition.y() < onScreenButtons.listButtons[i].screenPosition.y()
                                             + onScreenButtons.listButtons[i].scale.y()
            && clickScreenPosition.x() > onScreenButtons.listButtons[i].screenPosition.x()
            && clickScreenPosition.y() > onScreenButtons.listButtons[i].screenPosition.y()) {
            // Button clicked -> call onClick
            onScreenButtons.listButtons[i].onClickAction();
            return true;
        }
    }
    return false;
}

void test()
{
    std::cout << "test bouton" << std::endl;
}

void initWindow()
{
    //addButton();
    drawButtons();

    state = "NI";
}

void updateWindow()
{
    if (state == "NI") {
    }
    int x, y;

    // Get point mouse
    getMouse(x, y);

    detectClick(IntPoint2(x, y));
}
