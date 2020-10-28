#pragma once
#include "lodepng.h"
#include "Couleur.h"
#include "Lampe.h"
#include "Sphere.h"
#include <math.h>
#include <vector>

#define PI 3.141593

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height);

void normalizeColor(std::vector<unsigned char>* imageOut, std::vector<double>* image);

void color(std::vector<double>* img, int index, Couleur couleur);

Couleur colorOnSurface(Lampe lampe, Sphere* sphere);