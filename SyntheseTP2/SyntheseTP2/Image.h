#pragma once
#include "lodepng.h"
#include "Sphere.h"
#include <math.h>

#define PI 3.141593

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

void normalizeColor(std::vector<unsigned char>* imageOut, std::vector<double>* image);

void color(std::vector<double>* img, int index, Couleur couleur);

Couleur colorOnSurface(Lampe lampe, Sphere* sphere);