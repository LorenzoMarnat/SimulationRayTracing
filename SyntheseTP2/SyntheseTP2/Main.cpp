#include "lodepng.h"
#include "Mirroir.h"
#include "SphereCouleur.h"
#include "Lampe.h"
#include "Camera.h"
#include "Boite.h"
#include "Tree.h"
#include <math.h>
#include <iostream>
#include <random>

#define PI 3.141593
#define EPSILON -0.02
#define DEPTH 10

using namespace std;
default_random_engine gen;
uniform_real_distribution<double> distribution(-0.01, 0.01);
uniform_real_distribution<double> distributionLamp(-0.1, 0.1);

struct BoxTree {
	vector<Boite> boxes;
	Boite box;
	BoxTree* left = NULL;
	BoxTree* right = NULL;
};

struct BoxTree* NewBoxTree(vector<Boite> boxes, Boite box) {
	struct BoxTree* boxTree = new struct BoxTree();
	boxTree->box = Boite(box.min, box.max);
	boxTree->boxes = boxes;

	boxTree->left = NULL;
	boxTree->right = NULL;
	return boxTree;
};

struct BoxTree* boxTree;

void encodeOneStep(const char* filename, std::vector<unsigned char>& image, unsigned width, unsigned height) {
	//Encode the image
	unsigned error = lodepng::encode(filename, image, width, height);

	//if there's an error, display it
	if (error) std::cout << "encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
}

void normalizeColor(vector<unsigned char>* imageOut, vector<double>* image)
{
	double max = 0;
	for (int i = 0; i < (int)image->size(); i += 4)
	{
		if (image->at(i) > max)
			max = image->at(i);

		if (image->at(i + 1) > max)
			max = image->at(i + 1);

		if (image->at(i + 2) > max)
			max = image->at(i + 2);
	}

	max = max / 30;
	for (int i = 0; i < (int)image->size(); i += 4)
	{
		if ((int)((image->at(i) / max) * 255) > 255)
			imageOut->at(i) = 255;
		else
			imageOut->at(i) = (int)((image->at(i) / max) * 255);

		if ((int)((image->at(i + 1) / max) * 255) > 255)
			imageOut->at(i + 1) = 255;
		else
			imageOut->at(i + 1) = (int)((image->at(i + 1) / max) * 255);

		if ((int)((image->at(i + 2) / max) * 255) > 255)
			imageOut->at(i + 2) = 255;
		else
			imageOut->at(i + 2) = (int)((image->at(i + 2) / max) * 255);

		imageOut->at(i + 3) = image->at(i + 3);
	}

}
void color(vector<double>* img, int index, Couleur couleur)
{
	img->at(index) += couleur.red;

	img->at(index + 1) += couleur.green;

	img->at(index + 2) += couleur.blue;

	img->at(index + 3) = couleur.alpha;
}

Couleur colorOnSurface(Lampe lampe, Sphere* sphere)
{
	float distanceCarre = (lampe.GetDistance()) * (lampe.GetDistance());

	Vector3 normaleSurface = lampe.GetOrigin() - sphere->GetCentre();
	normaleSurface = normaleSurface.normalize();

	double scalaire = abs(normaleSurface.dot(lampe.GetDirection()));

	double intensity = (scalaire * lampe.intensity) / (PI * distanceCarre);

	return Couleur(sphere->albedo.red * intensity, sphere->albedo.green * intensity, sphere->albedo.blue * intensity);
}

void addSphere(vector<Sphere*>* spheres, Sphere* sphere)
{
	int size = spheres->size();
	spheres->resize(size + 1);
	spheres->at(size) = sphere;
}

void addLampe(vector<Lampe>* lampes, Lampe lampe)
{
	int size = lampes->size();
	lampes->resize(size + 1);
	lampes->at(size) = lampe;
}

bool raySphereIntersect(Rayon r, Sphere* s, float* distance) {
	bool intersect = false;

	/*cout << s->GetCentre().x << endl;
	cout << s->GetCentre().x << endl;
	cout << s->GetCentre().x << endl;*/
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
				return true;
			}
		}
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
		//for (int i = 0; i < 10; i++)
		//{
		lampe.SetOrigin(intersection);
		lampe.SetDirection(Vector3(lampe.GetDirection().x + distributionLamp(gen), lampe.GetDirection().y + distributionLamp(gen), lampe.GetDirection().z + distributionLamp(gen)));
		float distanceLampe = -1;
		int idSphere = -1; 
		isTreeIntersected(boxTree, lampe, 0, &spheres, &idSphere,&distanceLampe);
		if (idSphere == -1 || distanceLampe > lampe.GetDistance())
		{
			surface = colorOnSurface(lampe, spheres[intersectSphere]);
			color(image, index, surface);

			nbIntersection++;
		}
		//}
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
	isTreeIntersected(boxTree, rayonSortant, 0, spheres, &sphereIntersect,&minDistance);
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

void checkNormale(Vector3* n)
{
	if (n->x > 1)
		n->x = 1;
	if (n->x < -1)
		n->x = -1;

	if (n->y > 1)
		n->y = 1;
	if (n->y < -1)
		n->y = -1;

	if (n->z > 1)
		n->z = 1;
	if (n->z < -1)
		n->z = -1;
}

float RandomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

void sphereToBox(vector<Boite>* b, Sphere* s)
{
	Boite box = Boite(s);
	b->push_back(box);
}

bool pointInBox(Vector3 p, Boite box)
{
	if (p.x >= box.min.x && p.x <= box.max.x && p.y >= box.min.y && p.y <= box.max.y && p.z >= box.min.z && p.z <= box.max.z)
		return true;
	return false;
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

		if (pointInBox(tree->boxes[i].centre, leftBox))
			left.push_back(tree->boxes[i]);
		else
			if (pointInBox(tree->boxes[i].centre, rightBox))
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
				//color(image, index, intersectedSpheres->at(inter)->albedo);
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

int main(int argc, char* argv[]) {
	const char* filename = argc > 1 ? argv[1] : "test.png";

	int width = 1024, height = 1024;

	vector<unsigned char> imageOut;
	imageOut.resize(width * height * 4);

	vector<double> image;
	image.resize(width * height * 4);

	for (int i = 0; i < width * height * 4; i++)
		image[i] = 0;

	Camera camera = Camera((float)width, (float)height, 1024, Vector3(0, 0, 0));

	//vector<Sphere*> spheres;

	SphereCouleur rouge = SphereCouleur(200, Vector3(200, 0, 350), Couleur(1, 0, 0));

	SphereCouleur bleu = SphereCouleur(200, Vector3(200, 750, 350), Couleur(0, 0, 1));

	SphereCouleur vert = SphereCouleur(100, Vector3(850, 350, 150), Couleur(0, 1, 0));

	SphereCouleur cyan = SphereCouleur(150, Vector3(800, 800, 180), Couleur(0, 1, 1));

	SphereCouleur jaune = SphereCouleur(100, Vector3(0, 800, 120), Couleur(1, 1, 0));

	SphereCouleur sol = SphereCouleur(100000, Vector3(512, 101000, 500), Couleur(1, 0, 1));

	SphereCouleur mur = SphereCouleur(100000, Vector3(512, 512, -100200), Couleur(1, 0, 1));

	SphereCouleur plafond = SphereCouleur(100000, Vector3(512, -100500, 500), Couleur(1, 0, 1));

	SphereCouleur murFond = SphereCouleur(100000, Vector3(512, 512, 101000), Couleur(1, 0, 1));

	SphereCouleur murDroite = SphereCouleur(100000, Vector3(-100500, 512, 512), Couleur(1, 0, 1));

	SphereCouleur murGauche = SphereCouleur(100000, Vector3(101000, 512, 512), Couleur(1, 0, 1));

	Mirroir mirroir = Mirroir(150, Vector3(600, 450, 300), Couleur(1, 1, 1));

	Mirroir mirroir2 = Mirroir(100, Vector3(750, 100, 150), Couleur(1, 1, 1));

	/*addSphere(&spheres, &rouge);
	addSphere(&spheres, &bleu);
	addSphere(&spheres, &vert);
	addSphere(&spheres, &cyan);
	addSphere(&spheres, &jaune);*/

	/*addSphere(&spheres, &sol);
	addSphere(&spheres, &murDroite);
	addSphere(&spheres, &murFond);
	addSphere(&spheres, &murGauche);
	addSphere(&spheres, &mur);
	addSphere(&spheres, &plafond);*/

	//addSphere(&spheres, &mirroir);
	//addSphere(&spheres, &mirroir2);

	vector<Lampe> lampes;

	Lampe lampe1 = Lampe(Vector3(500, 500, 0), 10);

	Lampe lampe2 = Lampe(Vector3(100, 800, 0), 10);

	Lampe lampe3 = Lampe(Vector3(800, 100, 0), 10);

	addLampe(&lampes, lampe1);
	addLampe(&lampes, lampe2);
	addLampe(&lampes, lampe3);

	vector<Boite> boxes;
	vector<Sphere*>* spheres = new vector<Sphere*>();

	for (int i = 0; i < 10000; i++)
	{
		SphereCouleur* sphere = new SphereCouleur(10, Vector3(RandomFloat(0, 1000), RandomFloat(0, 1000), RandomFloat(11, 1000)), Couleur(RandomFloat(0.1, 1), RandomFloat(0.1, 1), RandomFloat(0.1, 1)), i);
		spheres->push_back(sphere);
		sphereToBox(&boxes, sphere);
	}

	Boite scene = Boite(Vector3(0, 0, 0), Vector3(1000, 1000, 1000));

	boxTree = NewBoxTree(boxes, scene);

	dichotomie(boxTree, 0, true);

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			float minDistance;

			Vector3 point = Vector3(camera.plan.x + x, camera.plan.y + y, camera.plan.z);
			Vector3 normale = camera.Normale(point);
			for (int i = 0; i < 1; i++)
			{
				//Vector3 normaleRand = Vector3(normale.x + distribution(gen), normale.y + distribution(gen), normale.z + distribution(gen));
				//checkNormale(&normaleRand);
				//Rayon rayon = Rayon(normaleRand, point);
				Rayon rayon = Rayon(normale, point);
				float distance;
				if (rayBoxIntersect(rayon, boxTree->box, &distance))
					intersectTree(boxTree, rayon, 0, &image, 4 * width * y + 4 * x, spheres, &lampes);
			}
		}
	}

	normalizeColor(&imageOut, &image);
	encodeOneStep(filename, imageOut, width, height);
}
