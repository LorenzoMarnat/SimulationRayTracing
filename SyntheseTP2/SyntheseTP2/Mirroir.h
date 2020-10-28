#pragma once
#include "Sphere.h"
class Mirroir :
    public Sphere
{
public:
	Mirroir(float _rayon, Vector3 _centre, Couleur _albedo, int _id);
	Mirroir(float _rayon, Vector3 _centre, Couleur _albedo);
	Mirroir();
	virtual bool IsMirror() { return true; }
};

