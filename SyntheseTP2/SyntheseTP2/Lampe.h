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

	Lampe(Vector3 _position, Vector3 _origine, float _intensity)
	{
		Vector3 _direction = _position - _origine;
		distance = _direction.length();
		_direction.normalize();
		position = _position;
		origine = _origine;
		direction = _direction;
		intersection = 0;
		intensity = _intensity;
	}
	Lampe(Vector3 _position, float _intensity)
	{
		//Vector3 _direction = _position - _origine;
		//distance = _direction.length();
		//_direction.normalize();
		position = _position;
		//origine = _origine;
		//direction = _direction;
		distance = 0;
		origine = Vector3(0, 0, 0);
		direction = Vector3(0, 0, 0);
		intersection = 0;
		intensity = _intensity;
	}
	Lampe(Vector3 _position, Vector3 _origine)
	{
		Vector3 _direction = _position - _origine;
		distance = _direction.length();
		_direction.normalize();
		position = _position;
		origine = _origine;
		direction = _direction;
		intersection = 0;
	}
	Lampe() : Rayon()
	{
		position = Vector3(0, 0, 0);
		distance = 0;
		intersection = 0;
	}

    Vector3 GetPosition() { return position; }

	float GetDistance() { return distance; }

	void SetOrigine(Vector3 _origine)
	{
		Vector3 _direction = position - _origine;
		distance = _direction.length();
		_direction.normalize();
		origine = _origine;
		direction = _direction;
	}

};

