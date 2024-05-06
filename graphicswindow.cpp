#include "graphicswindow.h"

void addButton(button buttonToAdd)
{
    // Add button to list of on screen buttons
    onScreenButtons.listButtons[onScreenButtons.lastFreeIndex] = buttonToAdd;
    onScreenButtons.lastFreeIndex += 1;
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
