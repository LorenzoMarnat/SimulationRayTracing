#pragma once
#include "vector3.h"
#include "Couleur.h"

class Rayon
{
private:
	Vector3 direction;
	Vector3 origine;
	Couleur couleur;

public:
	Rayon(Vector3 _direction, Vector3 _origine)
	{
		direction = _direction;
		origine = _origine;
		couleur = Couleur();
	}
	Rayon(Vector3 _direction, Vector3 _origine, Couleur _couleur)
	{
		direction = _direction;
		origine = _origine;
		couleur = Couleur(_couleur.red,_couleur.green,_couleur.blue,_couleur.alpha);
	}
	Rayon()
	{
		direction = Vector3(0,0,0);
		origine = Vector3(0, 0, 0);
	}

	Vector3 GetDirection() { return direction; }

	Vector3 GetOrigine() { return origine; }

	Couleur GetCouleur() { return couleur; }

	void SetDirection(Vector3 _direction) { direction = _direction; }

	void SetOrigine(Vector3 _origine) { origine = _origine; }

	void SetCouleur(Couleur _couleur) { couleur = _couleur; }
};

