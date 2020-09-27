#pragma once
#include "Sphere.h"
class Mirroir :
    public Sphere
{
public:
	Mirroir(float _rayon, Vector3 _centre, Couleur _albedo)
	{
		rayon = _rayon;
		centre = _centre;
		CheckAlbedo(&_albedo);
		albedo = _albedo;
	}
	Mirroir()
	{
		rayon = 0;
		centre = Vector3(0, 0, 0);
		albedo = Couleur(1, 1, 1);
	}

	virtual void PrintDebug() const { std::cout << "Mirroir" << std::endl; }
};

