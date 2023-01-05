#include "pch.h"
#include "Math.h"
#include "Renderer.h"

namespace bm98
{

#pragma region Vector2f

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
	return Vector2f(0, 0);
}

float Vector2f::distance(sf::Vector2f a, sf::Vector2f b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

float Vector2f::sqr_distance(sf::Vector2f a, sf::Vector2f b)
{
	return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

float Vector2f::dot_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

Vector2f Vector2f::get_normalized()
{
	if (sqr_magnitude() != 0)
	{
		return Vector2f(x / std::sqrt(sqr_magnitude()), y / std::sqrt(sqr_magnitude()));
	}
	return Vector2f(x, y);
}

void Vector2f::normalize()
{
	if (sqr_magnitude() != 0)
	{
		x = x / std::sqrt(sqr_magnitude());
		y = y / std::sqrt(sqr_magnitude());
	}
}

Json::Value Vector2f::serialize_json()
{
	Json::Value obj;

	obj["x"] = x;
	obj["y"] = y;

	return obj;
}

void Vector2f::unserialize_json(Json::Value obj)
{
	x = obj["x"].asInt64();
	y = obj["y"].asInt64();
}

#pragma endregion


#pragma region FloatConvex

FloatConvex::FloatConvex()
{
	
}

FloatConvex FloatConvex::circle(sf::Vector2f position, float radius, std::size_t point_count)
{
	FloatConvex convex;
	
	convex.shape_type = ShapeType::CIRCLE;
	
	convex.setPointCount(point_count);
	convex.point_count = point_count;
	convex.model.reserve(point_count);

	float ratio = 6.28f / (point_count - 1);

	for (std::size_t i = 0; i != point_count; i++)
	{
		float ang = ratio * i;
		Vector2f point;

		point.x = radius * std::sin(ang);
		point.y = radius * std::cos(ang);

		convex.model.push_back(point);

	}
	convex.set_position(position);

	return convex;
}

FloatConvex FloatConvex::capsule(sf::Vector2f position, sf::Vector2f size, Direction dir, std::size_t point_count)
{
	FloatConvex convex;

	convex.shape_type = ShapeType::CAPSULE;

	convex.setPointCount(point_count);
	convex.point_count = point_count;
	convex.model.reserve(point_count);

	float ratio = 6.28f / (point_count - 1);
	float radius = 0.f;
	if (dir == Direction::VERTICAL)
	{
		radius = size.x / 2.f;
	}
	else
	{
		radius = size.y / 2.f;
	}
	for (std::size_t i = 0; i != point_count; i++)
	{
		float ang = ratio * i;
		Vector2f point;

		point.x = radius * std::sin(ang) + position.x;
		point.y = radius * std::cos(ang) + position.y;

		if (dir == Direction::VERTICAL)
		{
			if (point.y > position.y)
				point.y += size.y / 2.f;
			if (point.y < position.y)
				point.y -= size.y / 2.f;
		}
		else
		{
			if (point.x > position.x)
				point.x += size.x / 2.f;
			if (point.x < position.x)
				point.x -= size.x / 2.f;
		}

		convex.model.push_back(point);
	}

	convex.set_position(position);

	return convex;
}

FloatConvex FloatConvex::polygon(sf::Vector2f position, std::vector<Vector2f> model)
{
	FloatConvex convex;

	convex.shape_type = ShapeType::POLYGON;

	convex.setPointCount(model.size());
	convex.point_count = model.size();
	convex.model = model;
	convex.set_position(position);

	return convex;
}

void FloatConvex::move(float x, float y)
{
	set_position(Vector2f(position.x + x, position.y + y));
}

void FloatConvex::set_position(sf::Vector2f position)
{
	// TODO: Set position of model to position
	this->position = position;

	for (std::size_t i = 0; i != model.size(); i++)
	{
		setPoint(i, model[i] + position);
	}

}

Vector2f FloatConvex::get_position()
{
	return position;
}

bool FloatConvex::intersects(FloatConvex convex)
{
	// TODO: Work out most efficient algorithm to check for intersection of convex shapes

	FloatConvex* poly1 = this;
	FloatConvex* poly2 = &convex;

	for (int shape = 0; shape < 2; shape++)
	{
		if (shape == 1)
		{
			poly1 = &convex;
			poly2 = this;
		}

		for (int a = 0; a < poly1->getPointCount(); a++)
		{
			int b = (a + 1) % poly1->getPointCount();
			Vector2f axisProj = Vector2f(-(poly1->getPoint(b).y - poly1->getPoint(a).y), (poly1->getPoint(b).x - poly1->getPoint(a).x));

			//work out min and max 1D points for r1
			float min_r1 = INFINITY, max_r1 = -INFINITY;
			for (int p = 0; p < poly1->getPointCount(); p++)
			{
				//calc dot product
				float q = Vector2f::dot_product(poly1->getPoint(p), axisProj);
				min_r1 = std::min(min_r1, q);
				max_r1 = std::max(max_r1, q);
			}

			float min_r2 = INFINITY, max_r2 = -INFINITY;
			for (int p = 0; p < poly2->getPointCount(); p++)
			{
				//calc dot product
				float q = Vector2f::dot_product(poly2->getPoint(p), axisProj);
				min_r2 = std::min(min_r2, q);
				max_r2 = std::max(max_r2, q);
			}

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			{
				setFillColor(sf::Color::Red);
				return false;
			}
		}


	}


	setFillColor(sf::Color::Green);
	return true;
}

bool FloatConvex::intersection(FloatConvex a, FloatConvex b)
{
	return a.intersects(b);
}

#pragma region IDATA


Json::Value FloatConvex::serialize_json()
{
	Json::Value obj;

	obj["point-count"] = point_count;
	obj["position"] = position.serialize_json();
	obj["shape-type"] = shapetype_to_string(shape_type);

	for (auto& p : model)
	{
		obj["model"].append(p.serialize_json());
	}

	return obj;
}

void FloatConvex::unserialize_json(Json::Value obj)
{
	point_count = obj["point-count"].asUInt64();
	position.unserialize_json(obj["position"]);
	shape_type = string_to_shapetype(obj["shape-type"].asString());

	model.reserve(point_count);
	int x = 0;
	for (Json::Value point : obj["model"])
	{
		model[x].unserialize_json(point);
		x++;
	}

}

#pragma endregion


#pragma endregion





}