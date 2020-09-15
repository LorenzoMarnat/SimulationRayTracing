#pragma once
#include "vector3.h"
class Sphere
{
private:
	float rayon;
	Vector3 centre;

public:
	Sphere(float _rayon, Vector3 _centre)
	{
		rayon = _rayon;
		centre = _centre;
	}
	Sphere()
	{
		rayon = 0;
		centre = Vector3(0, 0, 0);
	}

	float GetRayon() { return rayon; }

	Vector3 GetCentre() { return centre; }

	void SetRayon(float _rayon) { rayon = _rayon; }

	Vector3 SetCentre(Vector3 _centre) { centre = _centre; }
};

