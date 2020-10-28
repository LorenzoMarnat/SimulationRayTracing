#include "Rayon.h"
Rayon::Rayon(Vector3 _direction, Vector3 _origin, float _intensity)
{
	direction = _direction;
	origin = _origin;
	intensity = _intensity;
	rebound = 0;
}
Rayon::Rayon(Vector3 _direction, Vector3 _origin)
{
	direction = _direction;
	origin = _origin;
	intensity = 1;
	rebound = 0;
}
Rayon::Rayon()
{
	direction = Vector3(0, 0, 0);
	origin = Vector3(0, 0, 0);
	intensity = 1;
	rebound = 0;
}