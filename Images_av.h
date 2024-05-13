#pragma once
#include "utils.h"

typedef Image<Color> Img;
typedef Image<int> Indimg;
typedef Image<byte> Im;

class Images_av
{
    int max_w_compute; // Max width computed for the image (added seams)
    int max_h_compute; // Max height size computed for the image (added seams)

    int k = 1;        // Max horizontal gap between each pixel of a vertical seam
    Img loaded_image; // Original Image loaded by user

    Indimg energy;      // Energy of each pixel of the original image
    Img energy_colored; // Displayable version of Energy
    Img energy_heat_colored; // Heatmap of energy

    Indimg vert_seams; // Each pixel contains the seam at which point original pixel is removed
    Img vert_seams_heat_colored; // Displayable version of vert_seams
    // Extended version of seams index and image
    Indimg vertseams_extended;
    Img vertseams_extended_heat_colored; // Displayable version of vertseams_extended
    Img loaded_image_extended;

public:
    Images_av();
    void LoadImage(string path);
    void OpenImage(string mode, string option = "none");

    int height();
    int width();

    void ComputeEnergy(string energy_function = "gradient");

    void ComputeAllVerticalSeams();
    void FindVerticalSeam(int *x_path, Indimg &mapping_indices);
    void ComputeVerticalSeamsToAdd();

    // void ComputeVerticalSeams();
    void DisplaySeamTopImage(int *x_path);

    void ConvertIndImgTOImg(Indimg &input, Img &output, string method = "bw");
    void ConvertBWtoHEAT(Img &input_bw, Img &output_heat);

    void Chooseparttoremove(Color* rgb,int width,int height);
    void Chooseparttokeep(Color *rgb, int width, int height);

private:
    void tighten_image_width(Img &output);
    void ENERG_Gradient();
    void fillMapping(Indimg mappingIndices, string mode);
};
