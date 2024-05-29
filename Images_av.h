#pragma once
#include "utils.h"
#include <climits>

typedef Image<Color> Img;
typedef Image<int> Indimg;
typedef Image<byte> Im;

class Images_av
{
    int max_w_compute; // Max width computed for the image (added seams)
    int max_h_compute; // Max height size computed for the image (added seams)

    int k = 1;        // Max horizontal gap between each pixel of a vertical seam
    Img loaded_image; // Original Image loaded by user
    Img brush_layer;  // Brush image to supress/keep parts of the image

    Indimg energy;      // Energy of each pixel of the original image
    Img energy_colored; // Displayable version of Energy
    Img energy_heat_colored; // Heatmap of energy

    Indimg vert_seams; // Each pixel contains the seam at which point original pixel is removed
    Img vert_seams_heat_colored; // Displayable version of vert_seams
    // Extended version of seams index and image
    Indimg vertseams_extended;
    Img vertseams_extended_heat_colored; // Displayable version of vertseams_extended
    Img loaded_image_extended;

    Img displayedImg; // Diplayed image -> in case users wants to save it

public:
    Images_av();

    IntPoint2 size();
    void LoadImage(string path, string option = "none");
    void OpenImage(string mode, string option = "none");

    int height();
    int width();

    void ComputeEnergy(string energy_function = "gradient");

    void ClearBrushEnergy();
    void ColorBrushEnergy(IntPoint2 position, int scale = 5, Color col = GREEN);
    void ApplyBrushEnergy();

    void ComputeAllVerticalSeams();
    void FindVerticalSeam(int *x_path, Indimg &mapping_indices);
    void ComputeVerticalSeamsToAdd();

    void DisplaySeamTopImage(int *x_path);

    void ConvertIndImgTOImg(Indimg &input, Img &output, string method = "bw");
    void ConvertBWtoHEAT(Img &input_bw, Img &output_heat);

    void Chooseparttoremove(Color* rgb,int width,int height);
    void Chooseparttokeep(Color *rgb, int width, int height);
    void SaveImage();

private:
    void tighten_image_width(Img &output);
    void ENERG_Gradient();
    void ENERG_Entropy();
    void ENERG_Gradient2();
    void fillMapping(Indimg mappingIndices, string mode);
};
