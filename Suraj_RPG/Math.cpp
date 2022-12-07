#include "pch.h"
#include "Math.h"

namespace bm98
{

Vector2f::Vector2f()
{
	x = 0;
	y = 0;
}

Vector2f::Vector2f(float x, float y)
: sf::Vector2f(x, y)
{

}

Vector2f::Vector2f(sf::Vector2f vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

float Vector2f::sqr_magnitude()
{
	return std::pow(x, 2) + std::pow(y, 2);
}

Vector2f Vector2f::down()
{
	return Vector2f(0.f, 1.f);
}

Vector2f Vector2f::up()
{
	return Vector2f(0.f, -1.f);
}

Vector2f Vector2f::left()
{
	return Vector2f(-1.f, 0);
}

Vector2f Vector2f::right()
{
	return Vector2f(1.f, 0.f);
}

Vector2f Vector2f::zero()
{
	return Vector2f();
}

float Vector2f::distance(sf::Vector2f a, sf::Vector2f b)
{
	return std::sqrt( std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2) );
}

}