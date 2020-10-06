#pragma once
#include "vector3.h"
#include "Sphere.h"
class Boite
{
public:
	Vector3 min;
	Vector3 max;
	Vector3 centre;
	Couleur albedo;

	Boite(Sphere s)
	{
		float rayon = s.GetRayon();
		centre = s.GetCentre();
		min = Vector3(centre.x - rayon, centre.y - rayon, centre.z -rayon);
		max = Vector3(rayon + centre.x, rayon + centre.y, rayon + centre.z);
		albedo = s.albedo;
	}
	Boite(Vector3 _min, Vector3 _max)
	{
		min = _min;
		max = _max;
		centre = max - ((max-min)/2);
		albedo = Couleur(1, 1, 1);
	}

	Boite()
	{
		min = Vector3(0, 0, 0);
		max = Vector3(1, 1, 1);
		centre = max - ((max - min) / 2);
		albedo = Couleur(1, 1, 1);
	}
};

