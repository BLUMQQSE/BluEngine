#pragma once
namespace bm98
{

class Vector2f : public sf::Vector2f
{
public:
	Vector2f();
	Vector2f(float x, float y);
	Vector2f(sf::Vector2f vec);
	
	float sqr_magnitude();
	
	static Vector2f down();
	static Vector2f up();
	static Vector2f left();
	static Vector2f right();
	static Vector2f zero();

	static float distance(sf::Vector2f a, sf::Vector2f b);

};


}

