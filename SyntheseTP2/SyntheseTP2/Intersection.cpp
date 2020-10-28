#include "Intersection.h"

uniform_real_distribution<double> distributionLamp(-0.1, 0.1);
default_random_engine gen;

struct BoxTree* boxTree;

struct BoxTree* NewBoxTree(vector<Boite> boxes, Boite box) {
	struct BoxTree* boxTree = new struct BoxTree();
	boxTree->box = Boite(box.min, box.max);
	boxTree->boxes = boxes;

	boxTree->left = NULL;
	boxTree->right = NULL;
	return boxTree;
};

bool raySphereIntersect(Rayon r, Sphere* s, float* distance) {
	bool intersect = false;

	Vector3 s0 = s->GetCentre();
	float sr = s->GetRayon();
	float a = r.GetDirection().dot(r.GetDirection());
	Vector3 s0_r0 = r.GetOrigin() - s0;
	double b = 2.0 * r.GetDirection().dot(s0_r0);
	float c = s0_r0.dot(s0_r0) - (sr * sr);
	if (b * b - 4.0 * a * c >= 0.0)
	{
		*distance = (-b - sqrt((b * b) - 4.0 * a * c)) / (2.0 * a);
		if (*distance >= 0)
			intersect = true;
	}
	return intersect;
}

int intersectSpheres(Rayon r, vector<Sphere*> spheres, float* distance)
{
	int intersect = -1;
	for (int i = 0; i < (int)spheres.size(); i++)
	{
		float minDistance;
		bool inteSphere = raySphereIntersect(r, spheres[i], &minDistance);
		if (inteSphere)
		{
			if (intersect == -1)
			{
				intersect = i;
				*distance = minDistance;
			}
			else
			{
				if (minDistance < *distance)
				{
					intersect = i;
					*distance = minDistance;
				}
			}
		}
	}
	return intersect;
}

bool rayBoxIntersect(Rayon ray, Boite box, float* t)
{
	Vector3 dirfrac = Vector3(1 / ray.GetDirection().x, 1 / ray.GetDirection().y, 1 / ray.GetDirection().z);
	float t1 = (box.min.x - ray.GetOrigin().x) * dirfrac.x;
	float t2 = (box.max.x - ray.GetOrigin().x) * dirfrac.x;
	float t3 = (box.min.y - ray.GetOrigin().y) * dirfrac.y;
	float t4 = (box.max.y - ray.GetOrigin().y) * dirfrac.y;
	float t5 = (box.min.z - ray.GetOrigin().z) * dirfrac.z;
	float t6 = (box.max.z - ray.GetOrigin().z) * dirfrac.z;

	float tmin = max(max(min(t1, t2), min(t3, t4)), min(t5, t6));
	float tmax = min(min(max(t1, t2), max(t3, t4)), max(t5, t6));
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		*t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		*t = tmax;
		return false;
	}

	*t = tmin;
	return true;
}

vector<Sphere*>* intersectBoxes(Rayon r, vector<Boite> boxes, float* distance)
{
	vector<Sphere*>* intersectedSpheres = new vector<Sphere*>();
	int intersect = -1;
	for (int i = 0; i < (int)boxes.size(); i++)
	{
		float minDistance;
		bool inteSphere = rayBoxIntersect(r, boxes[i], &minDistance);
		if (inteSphere)
		{
			intersectedSpheres->push_back(boxes.at(i).sphere);
			if (intersect == -1)
			{
				intersect = i;
				*distance = minDistance;
			}
			else
			{
				if (minDistance < *distance)
				{
					intersect = i;
					*distance = minDistance;
				}
			}
		}
	}
	return intersectedSpheres;
}
bool isTreeIntersected(struct BoxTree* tree, Rayon ray, int depth, vector<Sphere*>* spheres, int* id, float* distance)
{
	if (depth == DEPTH)
	{
		float minDistance;
		vector<Sphere*>* intersectedSpheres = intersectBoxes(ray, tree->boxes, &minDistance);
		if (intersectedSpheres->size() > 0)
		{
			float distanceSphere;
			int inter = intersectSpheres(ray, *intersectedSpheres, &distanceSphere);
			if (inter != -1)
			{
				*id = intersectedSpheres->at(inter)->id;
				*distance = distanceSphere;

				delete intersectedSpheres;

				return true;
			}
		}
		delete intersectedSpheres;
	}
	else
	{
		float leftDistance;
		bool leftInter = rayBoxIntersect(ray, tree->left->box, &leftDistance);

		float rightDistance;
		bool rightInter = rayBoxIntersect(ray, tree->right->box, &rightDistance);

		if (leftInter && !rightInter)
			return isTreeIntersected(tree->left, ray, depth + 1, spheres, id, distance);

		if (rightInter && !leftInter)
			return isTreeIntersected(tree->right, ray, depth + 1, spheres, id, distance);

		if (leftInter && rightInter)
		{
			bool boxInter;
			if (leftDistance <= rightDistance)
			{
				boxInter = isTreeIntersected(tree->left, ray, depth + 1, spheres, id, distance);
				if (!boxInter)
					return isTreeIntersected(tree->right, ray, depth + 1, spheres, id, distance);
				else
					return true;
			}
			else
			{
				boxInter = isTreeIntersected(tree->right, ray, depth + 1, spheres, id, distance);
				if (!boxInter)
					return isTreeIntersected(tree->left, ray, depth + 1, spheres, id, distance);
				else
					return true;
			}
		}
	}
	return false;
}
void intersectLamps(Vector3 intersection, vector<Lampe> lamps, vector<Sphere*> spheres, int intersectSphere, vector<double>* image, int index)
{
	Couleur surface = Couleur(0, 0, 0);

	int nbIntersection = 0;

	for (Lampe lampe : lamps)
	{
		for (int i = 0; i < 10; i++)
		{
			lampe.SetOrigin(intersection);
			lampe.SetDirection(Vector3(lampe.GetDirection().x + distributionLamp(gen), lampe.GetDirection().y + distributionLamp(gen), lampe.GetDirection().z + distributionLamp(gen)));
			float distanceLampe = -1;
			int idSphere = -1;
			isTreeIntersected(boxTree, lampe, 0, &spheres, &idSphere, &distanceLampe);
			if (idSphere == -1 || distanceLampe > lampe.GetDistance())
			{
				surface = colorOnSurface(lampe, spheres[intersectSphere]);
				color(image, index, surface);

				nbIntersection++;
			}
		}
	}
	if (nbIntersection == 0)
	{
		color(image, index, Couleur(0, 0, 0));
	}
}
void mirrorRebound(Vector3 intersection, Rayon rayon, vector<Lampe>* lamps, vector<Sphere*>* spheres, int intersectSphere, vector<double>* image, int index)
{
	float minDistance;

	Vector3 normaleRayon = rayon.GetDirection();

	Vector3 normaleSurface = intersection - spheres->at(intersectSphere)->GetCentre();
	normaleSurface = normaleSurface.normalize();

	Vector3 directionRayon = (-normaleRayon.dot(normaleSurface)) * normaleSurface * 2 + normaleRayon;

	Rayon rayonSortant = Rayon(directionRayon, intersection);

	rayonSortant.rebound = ++rayon.rebound;

	int sphereIntersect = -1;
	isTreeIntersected(boxTree, rayonSortant, 0, spheres, &sphereIntersect, &minDistance);
	if (sphereIntersect != -1)
	{
		Vector3 newIntersection = Vector3(minDistance * normaleRayon.x + intersection.x, minDistance * normaleRayon.y + intersection.y, (float)(minDistance * normaleRayon.z + intersection.z));
		newIntersection += directionRayon * EPSILON;

		if (spheres->at(sphereIntersect)->IsMirror() && rayonSortant.rebound < Rayon::maxRebound)
		{
			mirrorRebound(newIntersection, rayonSortant, lamps, spheres, sphereIntersect, image, index);
		}
		else
		{
			intersectLamps(newIntersection, *lamps, *spheres, sphereIntersect, image, index);
		}
	}
	else
	{
		color(image, index, Couleur(0, 0, 0));
	}
}

void dichotomie(struct BoxTree* tree, int depth, bool vertical)
{

	vector<Boite> left;
	Boite leftBox;
	if (vertical)
		leftBox = Boite(tree->box.min, Vector3(tree->box.centre.x, tree->box.max.y, tree->box.max.z));
	else
		leftBox = Boite(tree->box.min, Vector3(tree->box.max.x, tree->box.centre.y, tree->box.max.z));

	vector<Boite> right;
	Boite rightBox;
	if (vertical)
		rightBox = Boite(Vector3(tree->box.centre.x, tree->box.min.y, tree->box.min.z), tree->box.max);
	else
		rightBox = Boite(Vector3(tree->box.min.x, tree->box.centre.y, tree->box.min.z), tree->box.max);

	for (int i = 0; i < (int)tree->boxes.size(); i++)
	{

		if (Boite::BoxInBox(tree->boxes[i], leftBox))
			left.push_back(tree->boxes[i]);

		if (Boite::BoxInBox(tree->boxes[i], rightBox))
			right.push_back(tree->boxes[i]);
	}

	tree->left = NewBoxTree(left, leftBox);

	tree->right = NewBoxTree(right, rightBox);
	if (depth < DEPTH)
	{
		dichotomie(tree->left, depth + 1, !vertical);
		dichotomie(tree->right, depth + 1, !vertical);
	}
}

bool intersectTree(struct BoxTree* tree, Rayon ray, int depth, vector<double>* image, int index, vector<Sphere*>* spheres, vector<Lampe>* lamps)
{
	if (depth == DEPTH)
	{
		float minDistance;
		vector<Sphere*>* intersectedSpheres = intersectBoxes(ray, tree->boxes, &minDistance);
		if (intersectedSpheres->size() > 0)
		{
			float distanceSphere;
			int inter = intersectSpheres(ray, *intersectedSpheres, &distanceSphere);
			if (inter != -1)
			{
				int idSphere = intersectedSpheres->at(inter)->id;
				Vector3 pointIntersection = Vector3(distanceSphere * ray.GetDirection().x + ray.GetOrigin().x, distanceSphere * ray.GetDirection().y + ray.GetOrigin().y, distanceSphere * ray.GetDirection().z + ray.GetOrigin().z);
				pointIntersection += ray.GetDirection() * EPSILON;
				if (!spheres->at(idSphere)->IsMirror())
				{
					intersectLamps(pointIntersection, *lamps, *spheres, idSphere, image, index);
				}
				else
				{
					mirrorRebound(pointIntersection, ray, lamps, spheres, idSphere, image, index);
				}
				delete intersectedSpheres;
				return true;
			}
			else
			{
				color(image, index, Couleur(0, 0, 0));
			}
		}
		else
		{
			color(image, index, Couleur(0, 0, 0));
		}
		delete intersectedSpheres;
	}
	else
	{
		float leftDistance;
		bool leftInter = rayBoxIntersect(ray, tree->left->box, &leftDistance);

		float rightDistance;
		bool rightInter = rayBoxIntersect(ray, tree->right->box, &rightDistance);

		if (leftInter && !rightInter)
			return intersectTree(tree->left, ray, depth + 1, image, index, spheres, lamps);

		if (rightInter && !leftInter)
			return intersectTree(tree->right, ray, depth + 1, image, index, spheres, lamps);

		if (leftInter && rightInter)
		{
			bool boxInter;
			if (leftDistance <= rightDistance)
			{
				boxInter = intersectTree(tree->left, ray, depth + 1, image, index, spheres, lamps);
				if (!boxInter)
					return intersectTree(tree->right, ray, depth + 1, image, index, spheres, lamps);
				else
					return true;
			}
			else
			{
				boxInter = intersectTree(tree->right, ray, depth + 1, image, index, spheres, lamps);
				if (!boxInter)
					return intersectTree(tree->left, ray, depth + 1, image, index, spheres, lamps);
				else
					return true;
			}
		}
	}
	return false;
}