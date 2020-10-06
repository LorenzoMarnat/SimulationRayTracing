#pragma once
#include "Boite.h"
#include "vector3.h"
class Tree
{

public:
	std::vector<Boite> boxes;
	Boite box;
	Tree* left;
	Tree* right;

	Tree(std::vector<Boite>* _boxes, Boite _box)
	{
		boxes = *_boxes;
		box = Boite(_box.min, _box.max);

		left = NULL;
	}
};

