#pragma once
#include <vector>
#include "Lampe.h"

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

	inline float GetRayon() { return rayon; }

	inline Vector3 GetCentre() { return centre; }

	inline void SetRayon(float _rayon) { rayon = _rayon; }

	inline void SetCentre(Vector3 _centre) { centre = _centre; }

	inline virtual bool IsMirror() { return false; }
};