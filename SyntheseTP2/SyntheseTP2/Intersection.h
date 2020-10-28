#pragma once
#include "Boite.h"
#include "Image.h"
#include <iostream>
#include <random>

#define EPSILON -0.02
#define DEPTH 10

using namespace std;


struct BoxTree {
	vector<Boite> boxes;
	Boite box;
	BoxTree* left = NULL;
	BoxTree* right = NULL;
};

extern struct BoxTree* boxTree;
extern default_random_engine gen;

struct BoxTree* NewBoxTree(vector<Boite> boxes, Boite box);

bool raySphereIntersect(Rayon r, Sphere* s, float* distance);

int intersectSpheres(Rayon r, vector<Sphere*> spheres, float* distance);

bool rayBoxIntersect(Rayon ray, Boite box, float* t);

vector<Sphere*>* intersectBoxes(Rayon r, vector<Boite> boxes, float* distance);
bool isTreeIntersected(struct BoxTree* tree, Rayon ray, int depth, vector<Sphere*>* spheres, int* id, float* distance);
void intersectLamps(Vector3 intersection, vector<Lampe> lamps, vector<Sphere*> spheres, int intersectSphere, vector<double>* image, int index);
void mirrorRebound(Vector3 intersection, Rayon rayon, vector<Lampe>* lamps, vector<Sphere*>* spheres, int intersectSphere, vector<double>* image, int index);

void dichotomie(struct BoxTree* tree, int depth, bool vertical);

bool intersectTree(struct BoxTree* tree, Rayon ray, int depth, vector<double>* image, int index, vector<Sphere*>* spheres, vector<Lampe>* lamps);