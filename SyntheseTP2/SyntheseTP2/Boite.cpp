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

bool Boite::PointInBox(Vector3 p, Boite box)
{
	if (p.x >= box.min.x && p.x <= box.max.x && p.y >= box.min.y && p.y <= box.max.y && p.z >= box.min.z && p.z <= box.max.z)
		return true;
	return false;
}

bool Boite::BoxInBox(Boite b, Boite box)
{
	Vector3 centre = b.centre;
	bool inBox = false;
	if (PointInBox(Vector3(centre.x + b.rayon, centre.y + b.rayon, centre.z + b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x + b.rayon, centre.y + b.rayon, centre.z - b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x + b.rayon, centre.y - b.rayon, centre.z + b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x + b.rayon, centre.y - b.rayon, centre.z - b.rayon), box))
		inBox = true;

	if (PointInBox(Vector3(centre.x - b.rayon, centre.y + b.rayon, centre.z + b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x - b.rayon, centre.y + b.rayon, centre.z - b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x - b.rayon, centre.y - b.rayon, centre.z + b.rayon), box))
		inBox = true;
	if (PointInBox(Vector3(centre.x - b.rayon, centre.y - b.rayon, centre.z - b.rayon), box))
		inBox = true;

	return inBox;
}