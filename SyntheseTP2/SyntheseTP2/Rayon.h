#pragma once
#include "vector3.h"

class Rayon
{
private:
	Vector3 direction;
	Vector3 origine;

public:
	Rayon(Vector3 _direction, Vector3 _origine)
	{
		direction = _direction;
		origine = _origine;
	}
	Rayon()
	{
		direction = Vector3(0,0,0);
		origine = Vector3(0, 0, 0);
	}

	Vector3 GetDirection() { return direction; }

	Vector3 GetOrigine() { return origine; }

	void SetDirection(Vector3 _direction) { direction = _direction; }

	void SetOrigine(Vector3 _origine) { origine = _origine; }
};

