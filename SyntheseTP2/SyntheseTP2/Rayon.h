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

	Rayon(Vector3 _direction, Vector3 _origin, float _intensity)
	{
		direction = _direction;
		origin = _origin;
		intensity = _intensity;
		rebound = 0;
	}
	Rayon(Vector3 _direction, Vector3 _origin)
	{
		direction = _direction;
		origin = _origin;
		intensity = 1;
		rebound = 0;
	}
	Rayon()
	{
		direction = Vector3(0,0,0);
		origin = Vector3(0, 0, 0);
		intensity = 1;
		rebound = 0;
	}

	Vector3 GetDirection() { return direction; }

	Vector3 GetOrigin() { return origin; }

	void SetDirection(Vector3 _direction) { direction = _direction; }

	void SetOrigin(Vector3 _origin) { origin = _origin; }
};

