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

	Lampe(Vector3 _position, Vector3 _origin, float _intensity)
	{
		Vector3 _direction = _position - _origin;
		distance = _direction.length();
		_direction.normalize();
		position = _position;
		origin = _origin;
		direction = _direction;
		intersection = 0;
		intensity = _intensity;
		rebound = 0;
	}
	Lampe(Vector3 _position, float _intensity)
	{
		position = _position;
		distance = 0;
		origin = Vector3(0, 0, 0);
		direction = Vector3(0, 0, 0);
		intersection = 0;
		intensity = _intensity;
		rebound = 0;
	}
	Lampe(Vector3 _position, Vector3 _origin)
	{
		Vector3 _direction = _position - _origin;
		distance = _direction.length();
		_direction.normalize();
		position = _position;
		origin = _origin;
		direction = _direction;
		intersection = 0;
		rebound = 0;
	}
	Lampe() : Rayon()
	{
		position = Vector3(0, 0, 0);
		distance = 0;
		intersection = 0;
	}

    Vector3 GetPosition() { return position; }

	float GetDistance() { return distance; }

	void SetOrigin(Vector3 _origin)
	{
		Vector3 _direction = position - _origin;
		distance = _direction.length();
		direction = _direction.normalize();
		origin = _origin;
	}

};

