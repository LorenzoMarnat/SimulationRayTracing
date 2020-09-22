#pragma once
#include "vector3.h"
#include "Couleur.h"

class Sphere
{
private:
	float rayon;
	Vector3 centre;

public:
	Couleur albedo;

	Sphere(float _rayon, Vector3 _centre,Couleur _couleur)
	{
		rayon = _rayon;
		centre = _centre;
		albedo = _couleur;
	}
	Sphere()
	{
		rayon = 0;
		centre = Vector3(0, 0, 0);
		albedo = Couleur(1,1,1);
	}

	float GetRayon() { return rayon; }

	Vector3 GetCentre() { return centre; }

	void SetRayon(float _rayon) { rayon = _rayon; }

	void SetCentre(Vector3 _centre) { centre = _centre; }
};

