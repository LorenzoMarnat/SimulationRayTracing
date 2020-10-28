#pragma once
class Couleur
{
private:
	void CheckColors(double* red, double* green, double* blue, float* alpha);

	void CheckColors(double* red, double* green, double* blue);
public:
	double blue;
	double red;
	double green;
	float alpha;

	Couleur(double _red, double _green, double _blue, float _alpha);
	Couleur(double _red, double _green, double _blue);
	Couleur();
	void operator=(Couleur c);

	Couleur operator+(Couleur c);

	Couleur operator*(Couleur c);

	Couleur operator/(int i);
};

