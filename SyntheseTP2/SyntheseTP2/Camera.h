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
	Camera(float width, float height, float depth, Vector3 _plan)
	{
		origine = Vector3(_plan.x + width / 2, _plan.y + height / 2, _plan.z - depth);
		plan = _plan;
		distance = 0;
	}

	Vector3 GetOrigine() { return origine; }

	Vector3 Normale(Vector3 point)
	{
		Vector3 direction = point - origine;
		distance = direction.length();
		return direction.normalize();
	}
};

