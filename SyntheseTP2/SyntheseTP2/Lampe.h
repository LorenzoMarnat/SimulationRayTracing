#pragma once
#include "Rayon.h"
class Lampe :
    public Rayon
{
private:
    Vector3 position;

public:
	Lampe(Vector3 _direction, Vector3 _origine, Vector3 _position) : Rayon(_direction,_origine)
	{
		position = _position;
	}
	Lampe(Vector3 _direction, Vector3 _origine, Vector3 _position, Couleur _couleur) : Rayon(_direction, _origine, _couleur)
	{
		position = _position;
	}
	Lampe() : Rayon()
	{
		position = Vector3(0, 0, 0);
	}

    Vector3 GetPosition() { return position; }

    void SetPosition(Vector3 _position) { position = _position; }
};

