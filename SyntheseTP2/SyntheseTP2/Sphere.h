#pragma once
#include "vector3.h"
#include "Couleur.h"
#include <vector>
#include "Lampe.h"
//#include "Singleton.h"

class Sphere
{
protected:
	float rayon;
	Vector3 centre;

	void CheckAlbedo(Couleur* _albedo);

public:
	Couleur albedo;
	int id;

	Sphere(float _rayon, Vector3 _centre, Couleur _albedo, int _id);

	Sphere(float _rayon, Vector3 _centre, Couleur _albedo);

	Sphere();

	float GetRayon() { return rayon; }

	Vector3 GetCentre() { return centre; }

	void SetRayon(float _rayon) { rayon = _rayon; }

	void SetCentre(Vector3 _centre) { centre = _centre; }

	virtual bool IsMirror() { return false; }
};