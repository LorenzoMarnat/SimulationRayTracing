#pragma once
#include "vector3.h"
#include "Couleur.h"
#include <vector>
#include "Lampe.h"
//#include "Singleton.h"

class Sphere
{
protected:
	float rayon;
	Vector3 centre;

	void CheckAlbedo(Couleur *_albedo)
	{
		if (_albedo->red > 1)
			_albedo->red = 1;
		if (_albedo->red < 0)
			_albedo->red = 0;

		if (_albedo->green > 1)
			_albedo->green = 1;
		if (_albedo->green < 0)
			_albedo->green = 0;

		if (_albedo->blue > 1)
			_albedo->blue = 1;
		if (_albedo->blue < 0)
			_albedo->blue = 0;
	}
public:
	Couleur albedo;

	Sphere(float _rayon, Vector3 _centre,Couleur _albedo)
	{
		rayon = _rayon;
		centre = _centre;
		CheckAlbedo(&_albedo);
		albedo = _albedo;
	}
	Sphere()
	{
		rayon = 0;
		centre = Vector3(0, 0, 0);
		albedo = Couleur(1,1,1);
	}

	float GetRayon() { return rayon; }

	Vector3 GetCentre() { return centre; }

	void SetRayon(float _rayon) { rayon = _rayon; }

	void SetCentre(Vector3 _centre) { centre = _centre; }

	virtual bool IsMirror() { return false; }
};