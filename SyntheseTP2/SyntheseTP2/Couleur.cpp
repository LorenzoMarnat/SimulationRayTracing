#include "Couleur.h"

void Couleur::CheckColors(double* red, double* green, double* blue, float* alpha)
{
	if (*red > 255)
		*red = 255;
	if (*red < 0)
		*red = 0;

	if (*green > 255)
		*green = 255;
	if (*green < 0)
		*green = 0;

	if (*blue > 255)
		*blue = 255;
	if (*blue < 0)
		*blue = 0;

	if (*alpha > 255)
		*alpha = 255;
	if (*alpha < 0)
		*alpha = 0;
}
void Couleur::CheckColors(double* red, double* green, double* blue)
{
	if (*red > 255)
		*red = 255;
	if (*red < 0)
		*red = 0;

	if (*green > 255)
		*green = 255;
	if (*green < 0)
		*green = 0;

	if (*blue > 255)
		*blue = 255;
	if (*blue < 0)
		*blue = 0;
}

Couleur::Couleur(double _red, double _green, double _blue, float _alpha)
{
	//CheckColors(&_red, &_green, &_blue, &_alpha);
	red = _red;
	green = _green;
	blue = _blue;
	alpha = _alpha;
}
Couleur::Couleur(double _red, double _green, double _blue)
{
	//CheckColors(&_red, &_green, &_blue);
	red = _red;
	green = _green;
	blue = _blue;
	alpha = 255;
}
Couleur::Couleur()
{
	red = 0;
	green = 0;
	blue = 0;
	alpha = 255;
}
void Couleur::operator=(Couleur c)
{
	red = c.red;
	green = c.green;
	blue = c.blue;
	alpha = c.alpha;
}

Couleur Couleur::operator+(Couleur c)
{
	Couleur result;
	result.red = red + c.red;
	result.blue = blue + c.blue;
	result.green = green + c.green;
	result.alpha = alpha;
	return result;
}

Couleur Couleur::operator*(Couleur c)
{
	Couleur result;
	result.red = red * c.red;
	result.blue = blue * c.blue;
	result.green = green * c.green;
	result.alpha = alpha;
	return result;
}

Couleur Couleur::operator/(int i)
{
	Couleur result;
	result.red = red / i;
	result.blue = blue / i;
	result.green = green / i;
	result.alpha = alpha;
	return result;
}