#pragma once
#include "Sphere.h"
class Boite
{
public:
	Vector3 min;
	Vector3 max;
	Vector3 centre;
	Couleur albedo;
	Sphere* sphere;
	float rayon;

	Boite(Sphere* s);

	Boite(Vector3 _min, Vector3 _max);

	Boite();

	static bool BoxInBox(Boite b, Boite box);
	static bool PointInBox(Vector3 p, Boite box);
};

