#pragma once
#include "Sphere.h"
class SphereCouleur :
    public Sphere
{
public:
	SphereCouleur(float _rayon, Vector3 _centre, Couleur _albedo, int _id);
	SphereCouleur(float _rayon, Vector3 _centre, Couleur _albedo);
	SphereCouleur();

	inline virtual bool IsMirror() { return false; }
};

