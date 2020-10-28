#include "Boite.h"

Boite::Boite(Sphere* s)
{
	rayon = s->GetRayon();
	centre = s->GetCentre();
	min = Vector3(centre.x - rayon, centre.y - rayon, centre.z - rayon);
	max = Vector3(rayon + centre.x, rayon + centre.y, rayon + centre.z);
	albedo = s->albedo;
	sphere = s;
}
Boite::Boite(Vector3 _min, Vector3 _max)
{
	min = _min;
	max = _max;
	centre = max - ((max - min) / 2);
	albedo = Couleur(1, 1, 1);
}

Boite::Boite()
{
	min = Vector3(0, 0, 0);
	max = Vector3(1, 1, 1);
	centre = max - ((max - min) / 2);
	albedo = Couleur(1, 1, 1);
}