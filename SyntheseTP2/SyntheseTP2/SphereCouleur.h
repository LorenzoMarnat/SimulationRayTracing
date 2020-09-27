#pragma once
#include "Sphere.h"
class SphereCouleur :
    public Sphere
{
public:
	SphereCouleur(float _rayon, Vector3 _centre, Couleur _albedo)
	{
		rayon = _rayon;
		centre = _centre;
		CheckAlbedo(&_albedo);
		albedo = _albedo;
	}
	SphereCouleur()
	{
		rayon = 0;
		centre = Vector3(0, 0, 0);
		albedo = Couleur(1, 1, 1);
	}

	virtual bool IsMirror() { return false; }
};

