#pragma once
#include "vector3.h"
#include "Couleur.h"

class Rayon
{
protected:
	Vector3 direction;
	Vector3 origine;
	//Couleur couleur;

public:
	float intensity;

	Rayon(Vector3 _direction, Vector3 _origine, float _intensity)
	{
		direction = _direction;
		origine = _origine;
		intensity = _intensity;
	}
	Rayon(Vector3 _direction, Vector3 _origine)
	{
		direction = _direction;
		origine = _origine;
		intensity = 1;
	}
	Rayon()
	{
		direction = Vector3(0,0,0);
		origine = Vector3(0, 0, 0);
		intensity = 1;
	}

	Vector3 GetDirection() { return direction; }

	Vector3 GetOrigine() { return origine; }

	void SetDirection(Vector3 _direction) { direction = _direction; }

	void SetOrigine(Vector3 _origine) { origine = _origine; }
};

