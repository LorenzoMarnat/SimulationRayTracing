#include "Intersection.h"
#include "Camera.h"
#include "SphereCouleur.h"
#include "Mirroir.h"
#include <chrono>
using namespace std;

uniform_real_distribution<double> distribution(-0.01, 0.01);

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


int main(int argc, char* argv[]) {
	// Start measuring time
	auto begin = std::chrono::high_resolution_clock::now();

	const char* filename = argc > 1 ? argv[1] : "test.png";

	int width = 1024, height = 1024;

	vector<unsigned char> imageOut;
	imageOut.resize(width * height * 4);

	vector<double> image;
	image.resize(width * height * 4);

	for (int i = 0; i < width * height * 4; i++)
		image[i] = 0;

	Camera camera = Camera((float)width, (float)height, 1024, Vector3(0, 0, 0));

	vector<Lampe> lampes;

	Lampe lampe1 = Lampe(Vector3(500, 500, 0), 10);

	Lampe lampe2 = Lampe(Vector3(100, 800, 0), 10);

	Lampe lampe3 = Lampe(Vector3(800, 100, 0), 10);

	lampes.push_back(lampe1);
	lampes.push_back(lampe2);
	lampes.push_back(lampe3);

	vector<Boite> boxes;
	vector<Sphere*>* spheres = new vector<Sphere*>();

	for (int i = 0; i < 50000; i++)
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
				Vector3 normaleRand = Vector3(normale.x + distribution(gen), normale.y + distribution(gen), normale.z + distribution(gen));
				checkNormale(&normaleRand);
				Rayon rayon = Rayon(normaleRand, point);

				//Rayon rayon = Rayon(normale, point);
				float distance;
				if (rayBoxIntersect(rayon, boxTree->box, &distance))
					intersectTree(boxTree, rayon, 0, &image, 4 * width * y + 4 * x, spheres, &lampes);
				else
					color(&image, 4 * width * y + 4 * x, Couleur(0, 0, 0));
			}
		}
	}

	normalizeColor(&imageOut, &image);
	encodeOneStep(filename, imageOut, width, height);

	// Stop measuring time and calculate the elapsed time
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);

	printf("Time measured: %.2f seconds.\n", elapsed.count() * 1e-9);
}
