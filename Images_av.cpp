#include "Images_av.h"
#include <cmath>
#include <iostream>
#include <vector>

Images_av::Images_av()
{
    cout << "Creating Advanced Image" << endl;
}

int Images_av::height(){
    return loaded_image.height();
}

int Images_av::width(){
    return loaded_image.width();
}


IntPoint2 Images_av::size()
{
    return IntPoint2{loaded_image.width(), loaded_image.height()};
}

void Images_av::LoadImage(string path, string option)
{
    if (option == "layer") {
        if (!load(brush_layer, stringSrcPath(path))) // Stop if image can't load
            cout << "Image couldn't be loaded" << endl;
        else if (brush_layer.width() != loaded_image.width()
                 || brush_layer.height() != loaded_image.height()) {
            cout << "Brush image not the same size as original image !" << endl;
        }
    } else {
        if (!load(loaded_image, stringSrcPath(path))) // Stop if image can't load
            cout << "Image couldn't be loaded" << endl;
        else {
            // Create energy Image
            energy = Indimg(loaded_image.width(), loaded_image.height());
            energy_colored = Img(loaded_image.width(), loaded_image.height());
            energy_heat_colored = Img(loaded_image.width(), loaded_image.height());
            vert_seams = Indimg(loaded_image.width(), loaded_image.height());
            vert_seams_heat_colored = Img(loaded_image.width(), loaded_image.height());
            brush_layer = Img(loaded_image.width(), loaded_image.height());
            // Create an extended version of seams index and image
            vertseams_extended = Indimg(vert_seams.width() * 2, vert_seams.height());
            vertseams_extended_heat_colored = Img(vert_seams_heat_colored.width() * 2,
                                                  vert_seams_heat_colored.height());
            loaded_image_extended = Img(loaded_image.width() * 2, loaded_image.height());

            cout << "Image succesfully loaded" << endl;
        }
    }
}

void Images_av::OpenImage(string mode, string option)
{
    if (mode == "original") {
        displayedImg = loaded_image;
    } else if (mode == "energy") {
        if (option == "bw") {
            ConvertIndImgTOImg(energy, energy_colored);
            displayedImg = energy_colored;
        } else if (option == "heat") {
            ConvertBWtoHEAT(energy_colored, energy_heat_colored);
            displayedImg = energy_heat_colored;
        }
    } else if (mode == "seams") {
        if (option == "extended") {
            displayedImg = vertseams_extended_heat_colored;
        } else {
            displayedImg = vert_seams_heat_colored;
        }
    } else if (mode == "size") {
        int new_width = std::stoi(option);
        Img new_image = Img(max(1, min(new_width, loaded_image_extended.width())),
                            loaded_image_extended.height());
        tighten_image_width(new_image);
        displayedImg = new_image;
    }
    display(displayedImg);
}

void Images_av::ComputeEnergy(string energy_function)
{
    if (energy_function == "gradient") {
        ENERG_Gradient();
    } else if (energy_function == "entropy") {
        ENERG_Entropy();
    } else if (energy_function == "gradient2") {
        ENERG_Gradient2();
    }

    ConvertIndImgTOImg(energy, energy_colored);
}

void Images_av::ClearBrushEnergy()
{
    for (int x = 0; x < brush_layer.width(); ++x) {
        for (int y = 0; y < brush_layer.height(); ++y) {
            brush_layer(x, y) = WHITE;
        }
    }
}

void Images_av::ColorBrushEnergy(IntPoint2 position, int scale, Color col)
{
    for (int x = max(0, position.x() - scale); x < min(position.x() + scale, brush_layer.width());
         ++x) {
        for (int y = max(0, position.y() - scale);
             y < min(position.y() + scale, brush_layer.height());
             ++y) {
            if ((position.x() - x) * (position.x() - x) + (position.y() - y) * (position.y() - y)
                < scale * scale)
                brush_layer(x, y) = col;
        }
    }
}

void Images_av::ApplyBrushEnergy()
{
    for (int y = 0; y < energy.height(); ++y) {
        for (int x = 0; x < energy.width(); ++x) {
            if (brush_layer(x, y) == GREEN) {
                energy(x, y) = (1 << 20);
            }
            if (brush_layer(x, y) == RED) {
                energy(x, y) = -(1 << 20);
            }
        }
    }
}

void Images_av::ComputeAllVerticalSeams()
{
    // INDICES relates to mapping_indices (0 to m_i.width) while Indices relates to image(mapping_indices) (0 to Image.width)
    Indimg mapping_indices = Indimg(loaded_image.width(),
                                    loaded_image.height()); // Mapping of column indices
    fillMapping(mapping_indices, "vertical");               // Fill initial mapping

    for (int removed_seam = 0; removed_seam < loaded_image.width(); ++removed_seam) {
        // Remove the i-th seam

        //// 1 -- Find INDICES to remove
        int *x_path = new int[loaded_image.height()]; // Will contain INDICES to remove
        FindVerticalSeam(x_path, mapping_indices);

        //// 2 -- Store information about which pixels have been removed for this seam
        for (int y = 0; y < mapping_indices.height(); ++y) {
            vert_seams(mapping_indices(x_path[y], y), y) = removed_seam;
        }

        //// 3 -- Update mapping_indices by removing the seam
        Indimg new_tmp_mapping_indices = Indimg(mapping_indices.width() - 1,
                                                mapping_indices.height());
        for (int y = 0; y < mapping_indices.height(); ++y) {
            for (int x = 0; x < x_path[y]; ++x) {
                new_tmp_mapping_indices(x, y) = mapping_indices(x, y);
            }
            for (int x = x_path[y]; x < new_tmp_mapping_indices.width(); ++x) {
                new_tmp_mapping_indices(x, y) = mapping_indices(x + 1, y);
            }
        }
        mapping_indices = new_tmp_mapping_indices;

        //// 4 -- Clear
        delete[] x_path;
    }

    ComputeVerticalSeamsToAdd();

    // Make displayable version
    ConvertIndImgTOImg(vert_seams, vert_seams_heat_colored, "heat");
    ConvertIndImgTOImg(vertseams_extended, vertseams_extended_heat_colored, "heat");
}

void Images_av::FindVerticalSeam(int *x_path, Indimg &mapping_indices)
{
    Indimg vertical_sum = Indimg(mapping_indices.width(), mapping_indices.height());
    int w = mapping_indices.width(), h = mapping_indices.height();

    //// 1 -- Dynamic Vertical Sum
    for (int y = 1; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            // Get minimum of previous pixels
            int min_pix = vertical_sum(x, y - 1);
            for (int i = std::max(0, x - k); i <= std::min(w - 1, x + k); ++i) {
                min_pix = std::min(min_pix, vertical_sum(i, y - 1));
            }

            vertical_sum(x, y) = (energy(mapping_indices(x, y), y) + min_pix);
        }
    }

    //// 2 -- Going Back
    /// a -- Get the last pixel of the minimum seam
    x_path[h - 1] = 0; // The INDEX corresponding to the column of minimum of last row in vertical_sum
    for (int i = 1; i < w; ++i) {
        if (vertical_sum(i, h - 1) < vertical_sum(x_path[h - 1], h - 1)) {
            x_path[h - 1] = i;
        }
    }
    /// b -- Going up the vertical_sum to find every previous pixel in the seam
    for (int y = h - 1; y > 0; --y) {
        // Find INDEX of column of previous pixel
        int prev_x_INDEX = x_path[y];
        for (int i = std::max(0, x_path[y] - k); i <= std::min(w - 1, x_path[y] + k); ++i) {
            if (vertical_sum(x_path[y], y)
                == (energy(mapping_indices(x_path[y], y), y) + vertical_sum(i, y - 1))) {
                prev_x_INDEX = i;
                break;
            }
        }
        x_path[y - 1] = prev_x_INDEX;
    }
}

void Images_av::ComputeVerticalSeamsToAdd()
{
    // Fill those new extended Img
    for (int y = 0; y < vertseams_extended.height(); ++y) {
        for (int i = 0; i < vertseams_extended.width(); ++i) {
            if (i % 2 == 0 || i == vertseams_extended.width() - 1) {
                vertseams_extended(i, y) = vert_seams(i / 2, y);
                loaded_image_extended(i, y) = loaded_image(i / 2, y);
            } else {
                vertseams_extended(i, y) = -vert_seams((i - 1) / 2, y);
                loaded_image_extended(i, y) = loaded_image((i - 1) / 2, y) / 2
                                              + loaded_image((i + 1) / 2, y) / 2;
            }
        }
    }
}

/* void Images_av::ComputeVerticalSeams()
{
    Indimg tmp = Image<int>(energy.width(), energy.height());
    Img tmp_colored = Image<Color>(energy.width(), energy.height());
    int wid = tmp.width(), hei = tmp.height();

    // Dynamic
    for (int y = 1; y < hei; ++y) {
        for (int x = 0; x < wid; ++x) {
            // Get minimum of previous pixels
            int min_pix = tmp(x, y - 1);
            for (int i = std::max(0, x - k); i <= std::min(wid - 1, x + k); ++i) {
                min_pix = std::min(min_pix, tmp(i, y - 1));
            }

            tmp(x, y) = (energy(x, y) + min_pix);
        }
    }

    // Backtrace
    // Créer un tableau d'indices dans l'ordre croissant
    std::vector<int> indices(energy.width());
    for (int i = 0; i < energy.width(); ++i) {
        indices[i] = i;
    }

    // Trier les indices en fonction des valeurs correspondantes dans le tableau
    std::sort(indices.begin(), indices.end(), [&tmp, &hei](int a, int b) {
        return tmp(a, hei - 1) < tmp(b, hei - 1);
    });
    // Find seam corresponding to minimum "energy sum" in last row
    int *x_path = new int[hei];   // Create the array that will store the x_coords of seam
    x_path[hei - 1] = indices[0]; // First pixel in seam corresponds to min on last row of tmp
    for (int y = hei - 1; y > 0; --y) {
        // Find x_coord of previous pixel
        int prev_x = x_path[y];
        for (int i = std::max(0, x_path[y] - k); i <= std::min(wid - 1, x_path[y] + k); ++i) {
            if (tmp(x_path[y], y) == (energy(x_path[y], y) + tmp(i, y - 1))) {
                prev_x = i;
                break;
            }
        }
        x_path[y - 1] = prev_x;
    }

    //     Display seam
    //        for (int y = 0; y < hei; ++y) {
    //            tmp(x_path[y], y) = 255;
    //        }
    //        display(tmp);

    ConvertIndImgTOImg(tmp, tmp_colored);

    DisplaySeamTopImage(x_path);
    delete[] x_path;
}
*/

void Images_av::DisplaySeamTopImage(int *x_path)
{
    Img tmp = Image<Color>(loaded_image.width(), loaded_image.height());
    for (int y = 0; y < tmp.height(); ++y) {
        for (int x = 0; x < tmp.width(); ++x) {
            tmp(x, y) = loaded_image(x, y) / 4.f;
        }
        tmp(x_path[y], y) = RED;
    }

    display(tmp);
}

void Images_av::ConvertIndImgTOImg(Indimg &input, Img &output, string method)
{
    if (method == "bw") {
        // Find minimum/maximum in img
        int min_value = 0, max_value = 0;
        for (int y = 0; y < input.height(); ++y) {
            for (int x = 0; x < input.width(); ++x) {
                if (input(x, y) > max_value)
                    max_value = input(x, y);
                if (input(x, y) < min_value)
                    min_value = input(x, y);
            }
        }

        // Convert to ouput
        for (int y = 0; y < input.height(); ++y) {
            for (int x = 0; x < input.width(); ++x) {
                int value = input(x, y) * 255.f / (max_value - min_value);
                output(x, y).r() = value;
                output(x, y).g() = value;
                output(x, y).b() = value;
            }
        }
    } else if (method == "heat") {
        // Start by making tmp version of image
        Img bw_tmp = Img(input.width(), input.height());
        ConvertIndImgTOImg(input, bw_tmp, "bw");
        // Convert to heat
        ConvertBWtoHEAT(bw_tmp, output);
    }
}

void Images_av::ConvertBWtoHEAT(Img &input_bw, Img &output_heat)
{
    for (int y = 0; y < input_bw.height(); ++y) {
        for (int x = 0; x < input_bw.width(); ++x) {
            int r, g, b;
            hslToRgb(255.f - distrib_2(input_bw(x, y).r(), 0.f, 255.f), 1.f, 0.5f, r, g, b);
            output_heat(x, y) = Color(r, g, b);
        }
    }
}

void Images_av::SaveImage()
{
    save(displayedImg, "export.png");
}

void Images_av::tighten_image_width(Img &output)
{
    int delta = (loaded_image.width() - output.width());
    for (int y = 0; y < loaded_image_extended.height(); ++y) {
        int added_pix_in_row = 0;
        for (int x = 0; x < loaded_image_extended.width(); ++x) {
            if (vertseams_extended(x, y) >= delta) {
                output(added_pix_in_row, y) = loaded_image_extended(x, y);
                added_pix_in_row++;
            }
            if (added_pix_in_row >= output.width() - 1) {
                break;
            }
        }
    }
}

//// ENERGY COMPUTING METHODS
/// Gradient
void Images_av::ENERG_Gradient()
{
    cout << "Computing Energy: Gradient" << endl;
    for (int y = 0; y < loaded_image.height(); ++y) {
        for (int x = 0; x < loaded_image.width(); ++x) {
            // sum each difference with adjacent pixel
            int red_h, red_v = 0;
            int green_h, green_v = 0;
            int blue_h, blue_v = 0;
            int pix_in_grad = 0;
            if (x > 0) {
                pix_in_grad++;
                red_h += loaded_image(x - 1, y).r() - loaded_image(x, y).r();
                green_h += loaded_image(x - 1, y).g() - loaded_image(x, y).g();
                blue_h += loaded_image(x - 1, y).b() - loaded_image(x, y).b();
            }
            if (x < loaded_image.width() - 1) {
                pix_in_grad++;
                red_h += loaded_image(x + 1, y).r() - loaded_image(x, y).r();
                green_h += loaded_image(x + 1, y).g() - loaded_image(x, y).g();
                blue_h += loaded_image(x + 1, y).b() - loaded_image(x, y).b();
            }
            if (y > 0) {
                pix_in_grad++;
                red_v += loaded_image(x, y - 1).r() - loaded_image(x, y).r();
                green_v += loaded_image(x, y - 1).g() - loaded_image(x, y).g();
                blue_v += loaded_image(x, y - 1).b() - loaded_image(x, y).b();
            }
            if (y < loaded_image.height() - 1) {
                pix_in_grad++;
                red_v += loaded_image(x, y + 1).r() - loaded_image(x, y).r();
                green_v += loaded_image(x, y + 1).g() - loaded_image(x, y).g();
                blue_v += loaded_image(x, y + 1).b() - loaded_image(x, y).b();
            }
            energy(x, y) = (std::abs(red_h) + std::abs(green_h) + std::abs(blue_h) + std::abs(red_v)
                            + std::abs(green_v) + std::abs(blue_v))
                           / (3 * pix_in_grad);
        }
    }
}

void Images_av::ENERG_Gradient2()
{
    cout << "Computing Energy: Gradient L2" << endl;
    for (int y = 0; y < loaded_image.height(); ++y) {
        for (int x = 0; x < loaded_image.width(); ++x) {
            // sum each difference with adjacent pixel
            int red_h, red_v = 0;
            int green_h, green_v = 0;
            int blue_h, blue_v = 0;
            int pix_in_grad = 0;
            if (x > 0) {
                pix_in_grad++;
                red_h += loaded_image(x - 1, y).r() - loaded_image(x, y).r();
                green_h += loaded_image(x - 1, y).g() - loaded_image(x, y).g();
                blue_h += loaded_image(x - 1, y).b() - loaded_image(x, y).b();
            }
            if (x < loaded_image.width() - 1) {
                pix_in_grad++;
                red_h += loaded_image(x + 1, y).r() - loaded_image(x, y).r();
                green_h += loaded_image(x + 1, y).g() - loaded_image(x, y).g();
                blue_h += loaded_image(x + 1, y).b() - loaded_image(x, y).b();
            }
            if (y > 0) {
                pix_in_grad++;
                red_v += loaded_image(x, y - 1).r() - loaded_image(x, y).r();
                green_v += loaded_image(x, y - 1).g() - loaded_image(x, y).g();
                blue_v += loaded_image(x, y - 1).b() - loaded_image(x, y).b();
            }
            if (y < loaded_image.height() - 1) {
                pix_in_grad++;
                red_v += loaded_image(x, y + 1).r() - loaded_image(x, y).r();
                green_v += loaded_image(x, y + 1).g() - loaded_image(x, y).g();
                blue_v += loaded_image(x, y + 1).b() - loaded_image(x, y).b();
            }
            energy(x, y) = std::sqrt(
                (std::pow((std::abs(red_h) + std::abs(green_h) + std::abs(blue_h)), 2)
                 + std::pow(std::abs(red_v) + std::abs(green_v) + std::abs(blue_v), 2))
                / std::pow((3 * pix_in_grad), 2));
        }
    }
}

void Images_av::ENERG_Entropy()
{
    cout << "Computing Energy: Entropy" << endl;
    // determination of histogram to determine probability
    int histogramr[256]; // each color have its own histogram
    int histogramb[256];
    int histogramg[256];
    int const N = loaded_image.width() * loaded_image.height();

    for (int y = 0; y < loaded_image.height(); ++y) {
        for (int x = 0; x < loaded_image.width(); ++x) {
            histogramr[loaded_image(x, y).r()] += 1;
            histogramb[loaded_image(x, y).b()] += 1;
            histogramg[loaded_image(x, y).g()] += 1;
        }
    }
    // determination of probability
    double pr[256];
    double pb[256];
    double pg[256];
    for (int i = 0; i < 256; i++) {
        pr[i] = histogramr[i] / N;
        pb[i] = histogramb[i] / N;
        pg[i] = histogramg[i] / N;
    }

    double probar;
    double probab;
    double probag;
    // calcul de l'entropy de Shannon pour chaque pixel de coordonnées (x,y)
    for (int y = 0; y < loaded_image.height(); ++y) {
        for (int x = 0; x < loaded_image.width(); ++x) {
            // Shannon Entropy
            // on prend les pixels autour pour detemriner l'entropie de chaque pixel (on pourrait essayer de determiner le nombre de pixel ideal a prendre)
            double Er = 0;
            double Eb = 0;
            double Eg = 0;
            double Etot = 0;
            probar = loaded_image(x, y).r();
            probab = loaded_image(x, y).b();
            probag = loaded_image(x, y).g();
            if (probar > 0) {
                Er -= probab * log2(probab);
            }
            if (probab > 0) {
                Eb -= probab * log2(probab);
            }
            if (probag > 0) {
                Eg -= probag * log2(probag);
            }
            if (x > 0) {
                probar = loaded_image(x - 1, y).r();
                probab = loaded_image(x - 1, y).b();
                probag = loaded_image(x - 1, y).g();
                if (probar > 0) {
                    Er -= probab * log2(probab);
                }
                if (probab > 0) {
                    Eb -= probab * log2(probab);
                }
                if (probag > 0) {
                    Eg -= probag * log2(probag);
                }
            }
            if (x < loaded_image.width() - 1) {
                probar = loaded_image(x + 1, y).r();
                probab = loaded_image(x + 1, y).b();
                probag = loaded_image(x + 1, y).g();
                if (probar > 0) {
                    Er -= probab * log2(probab);
                }
                if (probab > 0) {
                    Eb -= probab * log2(probab);
                }
                if (probag > 0) {
                    Eg -= probag * log2(probag);
                }
            }
            if (y > 0) {
                probar = loaded_image(x, y - 1).r();
                probab = loaded_image(x, y - 1).b();
                probag = loaded_image(x, y - 1).g();
                if (probar > 0) {
                    Er -= probab * log2(probab);
                }
                if (probab > 0) {
                    Eb -= probab * log2(probab);
                }
                if (probag > 0) {
                    Eg -= probag * log2(probag);
                }
            }
            if (y < loaded_image.height() - 1) {
                probar = loaded_image(x, y + 1).r();
                probab = loaded_image(x, y + 1).b();
                probag = loaded_image(x, y + 1).g();
                if (probar > 0) {
                    Er -= probab * log2(probab);
                }
                if (probab > 0) {
                    Eb -= probab * log2(probab);
                }
                if (probag > 0) {
                    Eg -= probag * log2(probag);
                }
            }
            Etot = Er + Eb + Eg;
            energy(x, y) = Etot;
        }
    }
}

void Images_av::fillMapping(Indimg mappingIndices, string mode)
{
    // In the case of vertical seams -> vertical lines are constant
    if (mode == "vertical") {
        for (int y = 0; y < mappingIndices.height(); ++y) {
            for (int x = 0; x < mappingIndices.width(); ++x) {
                mappingIndices(x, y) = x;
            }
        }
    } else {
        for (int y = 0; y < mappingIndices.height(); ++y) {
            for (int x = 0; x < mappingIndices.width(); ++x) {
                mappingIndices(x, y) = y;
            }
        }
    }
}

//            for (int i = std::max(0, index - k); i <= std::min(max_len - 1, index + k); ++i) {}

void Images_av::Chooseparttoremove(Color* rgb,int width, int height){
    for(int i=0;i<width;i++){
        for (int j=0;j<height;j++){
            if (rgb[i+width*j] == GREEN){
                energy(i,j) = -pow(2,20);
            }
        }
    }
}

void Images_av::Chooseparttokeep(Color* rgb,int width, int height){
    for(int i=0;i<width;i++){
        for (int j=0;j<height;j++){
            if (rgb[i+width*j] == RED){
                energy(i,j) = pow(2,20);
            }
        }
    }
}

