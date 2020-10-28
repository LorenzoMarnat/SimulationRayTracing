#pragma once
#include "vector3.h"
#include "Couleur.h"

class Rayon
{
protected:
	Vector3 direction;
	Vector3 origin;

public:
	float intensity;
	int rebound;
	static const int maxRebound = 5;

	Rayon(Vector3 _direction, Vector3 _origin, float _intensity);
	Rayon(Vector3 _direction, Vector3 _origin);
	Rayon();

	inline Vector3 GetDirection() { return direction; }

	inline Vector3 GetOrigin() { return origin; }

	inline void SetDirection(Vector3 _direction) { direction = _direction; }

	inline void SetOrigin(Vector3 _origin) { origin = _origin; }
};

