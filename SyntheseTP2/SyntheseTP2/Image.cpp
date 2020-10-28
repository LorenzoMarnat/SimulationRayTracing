#include "Image.h"
void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);
}

void normalizeColor(std::vector<unsigned char>* imageOut, std::vector<double>* image)
{
	double max = 0;
	for (int i = 0; i < (int)image->size(); i += 4)
	{
		if (image->at(i) > max)
			max = image->at(i);

		if (image->at(i + 1) > max)
			max = image->at(i + 1);

		if (image->at(i + 2) > max)
			max = image->at(i + 2);
	}

	max = max / 10;
	for (int i = 0; i < (int)image->size(); i += 4)
	{
		if ((int)((image->at(i) / max) * 255) > 255)
			imageOut->at(i) = 255;
		else
			imageOut->at(i) = (int)((image->at(i) / max) * 255);

		if ((int)((image->at(i + 1) / max) * 255) > 255)
			imageOut->at(i + 1) = 255;
		else
			imageOut->at(i + 1) = (int)((image->at(i + 1) / max) * 255);

		if ((int)((image->at(i + 2) / max) * 255) > 255)
			imageOut->at(i + 2) = 255;
		else
			imageOut->at(i + 2) = (int)((image->at(i + 2) / max) * 255);

		imageOut->at(i + 3) = image->at(i + 3);
	}

}
void color(std::vector<double>* img, int index, Couleur couleur)
{
	img->at(index) += couleur.red;

	img->at(index + 1) += couleur.green;

	img->at(index + 2) += couleur.blue;

	img->at(index + 3) = couleur.alpha;
}

Couleur colorOnSurface(Lampe lampe, Sphere* sphere)
{
	float distanceCarre = (lampe.GetDistance()) * (lampe.GetDistance());

	Vector3 normaleSurface = lampe.GetOrigin() - sphere->GetCentre();
	normaleSurface = normaleSurface.normalize();

	double scalaire = abs(normaleSurface.dot(lampe.GetDirection()));

	double intensity = (scalaire * lampe.intensity) / (PI * distanceCarre);

	return Couleur(sphere->albedo.red * intensity, sphere->albedo.green * intensity, sphere->albedo.blue * intensity);
}