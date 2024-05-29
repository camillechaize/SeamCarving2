#include "graphicswindow.h"
#include "graphics.h"

int window_w = 1200, window_h = 900;
buttonsList onScreenButtons;

string fileName_g;
Images_av working_image;
string state
    = "init"; // init / NI: no image loaded / IL: Image loaded / +E: energy computed / +S: seams computed / R: resize image by mouse
string energy = "gradient";
string brush = "remove";
int brushSize = 20;

void addButton(button buttonToAdd)
{
    buttonToAdd.screenPosition.x() = 950;
    buttonToAdd.screenPosition.y() = onScreenButtons.totalHeight + buttonToAdd.scale.y();

    // Add button to list of on screen buttons
    onScreenButtons.listButtons[onScreenButtons.lastFreeIndex] = buttonToAdd;
    onScreenButtons.totalHeight += buttonToAdd.scale.y();
    onScreenButtons.lastFreeIndex += 1;
}

void clearButtons()
{
    onScreenButtons.lastFreeIndex = 0;
    onScreenButtons.totalHeight = 0;
}

void drawButtons()
{
    // Clear Buttons
    fillRect(onScreenButtons.listButtons[0].screenPosition,
             onScreenButtons.listButtons[0].scale.x() + 5,
             900,
             WHITE);
    for (int i = 0; i < onScreenButtons.lastFreeIndex; ++i) {
        fillRect(onScreenButtons.listButtons[i].screenPosition,
                 onScreenButtons.listButtons[i].scale.x(),
                 onScreenButtons.listButtons[i].scale.y(),
                 Color(125, 125, 125));
        drawRect(onScreenButtons.listButtons[i].screenPosition,
                 onScreenButtons.listButtons[i].scale.x(),
                 onScreenButtons.listButtons[i].scale.y(),
                 BLACK);
        drawString(onScreenButtons.listButtons[i].screenPosition
                       + IntPoint2{4,
                                   (int) (onScreenButtons.listButtons[i].scale.y() * (2.0f / 3.0f))},
                   onScreenButtons.listButtons[i].text,
                   BLACK);
    }
}

void detectClick(IntPoint2 clickScreenPosition)
{
    if (!checkButtonClick(clickScreenPosition)) {
        // Check that click in image working area
        if (clickScreenPosition.x() < working_image.size().x() * 2
            && clickScreenPosition.y() < working_image.size().y()) {
            // If resize mode active, resize image by mouse click
            if (state == "R") {
                resizeImage(clickScreenPosition);
            } else if (state == "B") {
                Color col = brush == "keep" ? GREEN : RED;
                working_image.ColorBrushEnergy(clickScreenPosition, brushSize, col);
                fillCircle(clickScreenPosition, brushSize, col);
                fillRect(IntPoint2{0, working_image.size().y()}, 1200 - 250, 900, WHITE);
                fillRect(IntPoint2{working_image.size().x(), 0},
                         1200 - 250 - working_image.size().x(),
                         900,
                         WHITE);
                fillRect(IntPoint2{1200 - 250, onScreenButtons.totalHeight}, 250, 900, WHITE);
            }
        }
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

void initWindow(string fileName)
{
    openWindow(window_w, window_h);

    fileName_g = fileName;

    state = "NI";
    bool flag = true;

    while (flag) {
        updateWindow(flag);
    }

    endGraphics();
}

void updateWindow(bool &flag)
{
    clearButtons();

    if (state == "NI") {
        button loadImage_B = button{"Load Image",
                                    IntPoint2{0, 0},
                                    IntPoint2{250, 45},
                                    LoadImageButton};
        addButton(loadImage_B);
    } else if (state.find("IL") != std::string::npos) {
        clearButtons();
        button openOriginal_B = button{"Display Original",
                                       IntPoint2{0, 0},
                                       IntPoint2{250, 45},
                                       OpenOriginalButton};
        button calEnergy_B = button{"Calculate Energy",
                                    IntPoint2{0, 0},
                                    IntPoint2{250, 45},
                                    CalculateEnergyButton};
        button L1_norm_B = button{"     L1",
                                  IntPoint2{0, 0},
                                  IntPoint2{250, 45},
                                  ChangeEnergy_L1Button};
        button L2_norm_B = button{"     L2",
                                  IntPoint2{0, 0},
                                  IntPoint2{250, 45},
                                  ChangeEnergy_L2Button};
        button Entropy_norm_B = button{"     Entropy",
                                       IntPoint2{0, 0},
                                       IntPoint2{250, 45},
                                       ChangeEnergy_EntropyButton};

        button open_B = button{"Display Energy - BW",
                               IntPoint2{0, 0},
                               IntPoint2{250, 45},
                               ChangeEnergy_L2Button};

        addButton(openOriginal_B);
        addButton(calEnergy_B);
        addButton(L1_norm_B);
        addButton(L2_norm_B);
        addButton(Entropy_norm_B);
    }

    if (state.find("+E") != std::string::npos) {
        button open_E_BW_B = button{"Display Energy - BW",
                                    IntPoint2{0, 0},
                                    IntPoint2{250, 45},
                                    Open_E_BWButton};
        button open_E_Heat_B = button{"Display Energy - Heat",
                                      IntPoint2{0, 0},
                                      IntPoint2{250, 45},
                                      Open_E_HeatButton};
        addButton(open_E_BW_B);
        addButton(open_E_Heat_B);

        // Brush out area
        button brush_enter = button{"Brush", IntPoint2{0, 0}, IntPoint2{250, 45}, BrushEnterButton};
        addButton(brush_enter);

        // Select Zone to keep or remove area

        button SelectZone_enter = button{"Select Zone", IntPoint2{0, 0}, IntPoint2{250, 45}, SelectZoneEnterButton};
        addButton(SelectZone_enter);

        // Seam Carving button
        button seam_carving = button{"Compute Seams",
                                     IntPoint2{0, 0},
                                     IntPoint2{250, 45},
                                     SeamCarving};
        addButton(seam_carving);
    }
    if (state.find("+S") != std::string::npos) {
        button open_steps_B = button{"Display Steps",
                                     IntPoint2{0, 0},
                                     IntPoint2{250, 45},
                                     Open_StepButton};
        addButton(open_steps_B);
        button rezise_enter_B = button{"Resize Image",
                                       IntPoint2{0, 0},
                                       IntPoint2{250, 45},
                                       ResizeEnterButton};
        addButton(rezise_enter_B);
        button export_B = button{"Export Image", IntPoint2{0, 0}, IntPoint2{250, 45}, ExportImage};
        addButton(export_B);
    }
    if (state == "R") {
        clearButtons();
        button resize_exit_B = button{"Exit Resize",
                                      IntPoint2{0, 0},
                                      IntPoint2{250, 45},
                                      ResizeExitButton};
        addButton(resize_exit_B);
    }

    if (state == "X") {
        clearButtons();
        button resize_exit_X = button{"Exit Resize",
                                      IntPoint2{0, 0},
                                      IntPoint2{250, 45},
                                      BrushExitButton};
        addButton(resize_exit_X);
    }

    if (state == "B") {
        clearButtons();
        button brush_keep_B = button{"Keep", IntPoint2{0, 0}, IntPoint2{250, 45}, BrushKeepButton};
        button brush_remove_B = button{"Remove",
                                       IntPoint2{0, 0},
                                       IntPoint2{250, 45},
                                       BrushRemoveButton};
        button brush_5_B = button{"Size - 5", IntPoint2{0, 0}, IntPoint2{250, 45}, Brush5Button};
        button brush_20_B = button{"Size - 20", IntPoint2{0, 0}, IntPoint2{250, 45}, Brush20Button};
        button brush_50_B = button{"Size - 50", IntPoint2{0, 0}, IntPoint2{250, 45}, Brush50Button};
        button brush_200_B = button{"Size - 200",
                                    IntPoint2{0, 0},
                                    IntPoint2{250, 45},
                                    Brush200Button};
        button brush_exit_B = button{"Exit Brush",
                                     IntPoint2{0, 0},
                                     IntPoint2{250, 45},
                                     BrushExitButton};
        addButton(brush_keep_B);
        addButton(brush_remove_B);
        addButton(brush_5_B);
        addButton(brush_20_B);
        addButton(brush_50_B);
        addButton(brush_200_B);
        addButton(brush_exit_B);
    }

    drawButtons();

    int x, y;
    getMouse(x, y);
    detectClick(IntPoint2(x, y));
}

// BUTTON FUNCTIONS
void LoadImageButton()
{
    working_image.LoadImage(fileName_g);
    working_image.OpenImage("original");

    state = "IL";
}

void OpenOriginalButton()
{
    clearImageArea();
    working_image.OpenImage("original");
}

void CalculateEnergyButton()
{
    working_image.ComputeEnergy(energy);
    state = "IL+E";
}

void ChangeEnergy_L1Button()
{
    energy = "gradient";
}

void ChangeEnergy_L2Button()
{
    energy = "gradient2";
}
void ChangeEnergy_EntropyButton()
{
    energy = "entropy";
}

void Open_E_BWButton()
{
    clearImageArea();
    working_image.OpenImage("energy", "bw");
}

void Open_E_HeatButton()
{
    clearImageArea();
    working_image.OpenImage("energy", "heat");
}

void BrushEnterButton()
{
    working_image.ClearBrushEnergy();
    clearImageArea();
    working_image.OpenImage("original");
    state = "B";
}

void SelectZoneEnterButton()
{
    working_image.ClearBrushEnergy();
    clearImageArea();
    working_image.OpenImage("original");
    state = "X";
    int width = working_image.width();
    int height = working_image.height();
    Color* rgb = new Color[width*height];
    get_parts(width,height,rgb);
    working_image.Chooseparttokeep(rgb,width,height);
    working_image.Chooseparttoremove(rgb,width,height);
    working_image.OpenImage("original");
}

void BrushKeepButton()
{
    brush = "keep";
}
void BrushRemoveButton()
{
    brush = "remove";
}

void Brush5Button()
{
    brushSize = 5;
}
void Brush20Button()
{
    brushSize = 20;
}
void Brush50Button()
{
    brushSize = 50;
}
void Brush200Button()
{
    brushSize = 200;
}

void BrushExitButton()
{
    working_image.ApplyBrushEnergy();
    state = "IL+E";
}

void SelectZone(){

}

void SeamCarving()
{
    working_image.ComputeAllVerticalSeams();
    state = "IL+E+S";
}

void Open_StepButton()
{
    clearImageArea();
    working_image.OpenImage("seams");
}

void ResizeEnterButton()
{
    state = "R";
}

void ResizeExitButton()
{
    state = "IL+E+S";
}

void resizeImage(IntPoint2 clickScreenPosition)
{
    clearImageArea();
    working_image.OpenImage("size", std::to_string(clickScreenPosition.x()));
}

void clearImageArea()
{
    fillRect(IntPoint2{0, 0}, working_image.size().x() * 2, working_image.size().y(), WHITE);
}

void ExportImage()
{
    working_image.SaveImage();
}
