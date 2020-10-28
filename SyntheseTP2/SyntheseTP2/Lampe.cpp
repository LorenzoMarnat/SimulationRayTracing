#include "Lampe.h"
Lampe::Lampe(Vector3 _position, Vector3 _origin, float _intensity)
{
	Vector3 _direction = _position - _origin;
	distance = _direction.length();
	direction = _direction.normalize();
	origin = _origin;

	position = _position;

	intersection = 0;
	intensity = _intensity;
	rebound = 0;
}
Lampe::Lampe(Vector3 _position, float _intensity)
{
	position = _position;
	distance = 0;
	origin = Vector3(0, 0, 0);
	direction = Vector3(0, 0, 0);
	intersection = 0;
	intensity = _intensity;
	rebound = 0;
}
Lampe::Lampe(Vector3 _position, Vector3 _origin)
{
	Vector3 _direction = _position - _origin;
	distance = _direction.length();
	direction = _direction.normalize();
	position = _position;
	origin = _origin;
	intersection = 0;
	rebound = 0;
}

void Lampe::SetOrigin(Vector3 _origin)
{
	Vector3 _direction = position - _origin;
	distance = _direction.length();
	direction = _direction.normalize();
	origin = _origin;
}