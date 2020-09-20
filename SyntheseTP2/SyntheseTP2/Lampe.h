#pragma once
#include "Rayon.h"
class Lampe :
    public Rayon
{
private:
    Vector3 position;

public:
	Lampe(Vector3 _position, Vector3 _origine, Couleur _couleur)
	{
		Vector3 _direction = _position - _origine;
		_direction.normalize();
		position = _position;
		origine = _origine;
		direction = _direction;
		couleur = _couleur;
	}
	Lampe(Vector3 _position, Vector3 _origine)
	{
		Vector3 _direction = _position - _origine;
		_direction.normalize();
		position = _position;
		origine = _origine;
		direction = _direction;
		couleur = Couleur();
	}
	Lampe() : Rayon()
	{
		position = Vector3(0, 0, 0);
	}

    Vector3 GetPosition() { return position; }

    void SetPosition(Vector3 _position) { position = _position; }
};

