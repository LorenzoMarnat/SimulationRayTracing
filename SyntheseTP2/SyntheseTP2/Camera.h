#pragma once
#include "vector3.h"

class Camera
{
private:
	Vector3 origine;
public:
	Vector3 plan;
	float distance
		;
	Camera(float width, float height, float depth, Vector3 _plan);

	inline Vector3 GetOrigine() { return origine; }

	Vector3 Normale(Vector3 point);
};

