#pragma once
class Couleur
{
private:
	void CheckColors(float* red, float* green, float* blue, float* alpha)
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
	void CheckColors(float* red, float* green, float* blue)
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
public:
	float blue;
	float red;
	float green;
	float alpha;
	float intensity;

	Couleur(float _red, float _green, float _blue, float _alpha, float _intensity)
	{
		CheckColors(&_red, &_green, &_blue, &_alpha);
		red = _red;
		green = _green;
		blue = _blue;
		alpha = _alpha;
		intensity = _intensity;
	}
	Couleur(float _red, float _green, float _blue, float _alpha)
	{
		CheckColors(&_red, &_green, &_blue, &_alpha);
		red = _red;
		green = _green;
		blue = _blue;
		alpha = _alpha;
		intensity = 1;
	}
	Couleur(float _red, float _green, float _blue)
	{
		CheckColors(&_red, &_green, &_blue);
		red = _red;
		green = _green;
		blue = _blue;
		alpha = 255;
		intensity = 1;
	}
	Couleur()
	{
		red = 255;
		green = 255;
		blue = 255;
		alpha = 255;
		intensity = 1;
	}
	void operator=(Couleur c)
	{
		red = c.red;
		green = c.green;
		blue = c.blue;
		alpha = c.alpha;
		intensity = c.intensity;
	}
};

