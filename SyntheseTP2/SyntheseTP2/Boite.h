#pragma once
#include "vector3.h"
#include "Sphere.h"
class Boite
{
public:
	Vector3 p1;
	Vector3 p2;
	Couleur albedo;

	Boite(Sphere s)
	{
		float rayon = s.GetRayon();
		Vector3 centre = s.GetCentre();
		p1 = Vector3(centre.x - rayon, centre.y - rayon, centre.z -rayon);
		p2 = Vector3(rayon + centre.x, rayon + centre.y, rayon + centre.z);
		albedo = s.albedo;
	}
};

