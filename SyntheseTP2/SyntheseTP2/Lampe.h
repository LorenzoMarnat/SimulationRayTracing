#pragma once
#include "Rayon.h"
class Lampe :
    public Rayon
{
private:
    Vector3 position;
	float distance;

public:
	int intersection;

	Lampe(Vector3 _position, Vector3 _origin, float _intensity);
	Lampe(Vector3 _position, float _intensity);
	Lampe(Vector3 _position, Vector3 _origin);
	Lampe() : Rayon()
	{
		position = Vector3(0, 0, 0);
		distance = 0;
		intersection = 0;
	}

    Vector3 GetPosition() { return position; }

	float GetDistance() { return distance; }

	void SetDirection(Vector3 _direction)
	{
		direction = _direction;
	}

	void SetOrigin(Vector3 _origin);

};

