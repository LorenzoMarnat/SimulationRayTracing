#include "Mirroir.h"
Mirroir::Mirroir(float _rayon, Vector3 _centre, Couleur _albedo, int _id)
{
	rayon = _rayon;
	centre = _centre;
	CheckAlbedo(&_albedo);
	albedo = _albedo;
	id = _id;
}
Mirroir::Mirroir(float _rayon, Vector3 _centre, Couleur _albedo)
{
	rayon = _rayon;
	centre = _centre;
	CheckAlbedo(&_albedo);
	albedo = _albedo;
	id = -1;
}
Mirroir::Mirroir()
{
	rayon = 0;
	centre = Vector3(0, 0, 0);
	albedo = Couleur(1, 1, 1);
	id = -1;
}