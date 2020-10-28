#include "Sphere.h"

Sphere::Sphere(float _rayon, Vector3 _centre, Couleur _albedo, int _id)
{
	rayon = _rayon;
	centre = _centre;
	CheckAlbedo(&_albedo);
	albedo = _albedo;
	id = _id;
}
Sphere::Sphere(float _rayon, Vector3 _centre, Couleur _albedo)
{
	rayon = _rayon;
	centre = _centre;
	CheckAlbedo(&_albedo);
	albedo = _albedo;
	id = -1;
}
Sphere::Sphere()
{
	rayon = 0;
	centre = Vector3(0, 0, 0);
	albedo = Couleur(1, 1, 1);
	id = -1;
}

void Sphere::CheckAlbedo(Couleur* _albedo)
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