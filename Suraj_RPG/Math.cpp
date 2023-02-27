#include "pch.h"
#include "Math.h"
#include "Renderer.h"

namespace bm98
{

float PI = 3.14159265358979323846;
float RAD2DEG = 180.0 / PI;
float DEG2RAD = PI / 180.0;

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

float Vector2f::cross_product(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.y - a.y * b.x;
}

bool Vector2f::equal(sf::Vector2f a, sf::Vector2f b)
{
	if (a.x == b.x && a.y == b.y)
		return true;

	return false;
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

std::vector<Vector2f> Vector2f::get_normals(Vector2 a, Vector2f b)
{
	std::vector<Vector2f> normals;
	normals.push_back(Vector2(-(a.y - a.y), (b.x - a.x)));
	normals.push_back(Vector2((b.y-a.y), -(b.x-a.x)));
	return normals;
}

bool Vector2f::equals(sf::Vector2f a)
{
	if (x == a.x && y == a.y)
		return true;
	return false;
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
	x = obj["x"].asFloat();
	y = obj["y"].asFloat();
}

#pragma endregion


#pragma region Vector2i

Vector2i::Vector2i()
{
	x = 0;
	y = 0;
}

Vector2i::Vector2i(int x, int y)
	: sf::Vector2i(x, y)
{

}

Vector2i::Vector2i(sf::Vector2i vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

int Vector2i::sqr_magnitude()
{
	return std::pow(x, 2) + std::pow(y, 2);
}

Vector2i Vector2i::down()
{
	return Vector2i(0.f, 1.f);
}

Vector2i Vector2i::up()
{
	return Vector2i(0.f, -1.f);
}

Vector2i Vector2i::left()
{
	return Vector2i(-1.f, 0);
}

Vector2i Vector2i::right()
{
	return Vector2i(1.f, 0.f);
}

Vector2i Vector2i::zero()
{
	return Vector2i(0, 0);
}

int Vector2i::distance(sf::Vector2i a, sf::Vector2i b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int Vector2i::sqr_distance(sf::Vector2i a, sf::Vector2i b)
{
	return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

int Vector2i::dot_product(sf::Vector2i a, sf::Vector2i b)
{
	return a.x * b.x + a.y * b.y;
}

bool Vector2i::equal(sf::Vector2i a, sf::Vector2i b)
{
	if (a.x == b.x && a.y == b.y)
		return true;
	return false;
}

bool Vector2i::equals(sf::Vector2i a)
{
	if (x == a.x && y == a.y)
		return true;
	return false;
}

Json::Value Vector2i::serialize_json()
{
	Json::Value obj;

	obj["x"] = x;
	obj["y"] = y;

	return obj;
}

void Vector2i::unserialize_json(Json::Value obj)
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

FloatConvex FloatConvex::line(sf::Vector2f start, sf::Vector2f end, float thickness)
{
	FloatConvex convex;

	Vector2f normal = Vector2f::get_normals(start, end)[0];

	convex.shape_type = ShapeType::LINE;

	convex.setPointCount(4);
	convex.point_count = 4;
	convex.model.push_back(Vector2f(0, 0));
	convex.model.push_back(normal.get_normalized() * thickness);
	convex.model.push_back((end - start) + normal.get_normalized() * thickness);
	convex.model.push_back(end - start);
	convex.set_position(start);

	return convex;
}

FloatConvex FloatConvex::combine_shapes(FloatConvex a, FloatConvex b)
{
	FloatConvex convex;

	if (a.position.x < b.position.x)
		convex.set_position(a.position);
	else
		convex.set_position(b.position);

	convex.model.insert(convex.model.end(), a.model.begin(), a.model.end());
	convex.model.insert(convex.model.end(), b.model.begin(), b.model.end());


	convex = sort_poly_model(convex);

	// remove duplicates
	
	//std::unique(convex.model.begin(), convex.model.end());




	convex.setPointCount(convex.model.size());
	convex.point_count = convex.model.size();

	convex.shape_type = ShapeType::POLYGON;
	
	Vector2f center = convex.get_center();

	return convex;
}

void FloatConvex::move(float x, float y)
{
	set_position(Vector2f(position.x + x, position.y + y));
}

void FloatConvex::set_position(sf::Vector2f position)
{
	// TODO: Set position of model to positionwwwwwwww
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

Vector2f FloatConvex::get_center()
{
	Vector2f c;

	for (int p = 0; p < getPointCount(); p++)
	{
		c += getPoint(p);
	}
	c.x /= getPointCount();
	c.y /= getPointCount();
	return c;
}

bool FloatConvex::intersects(FloatConvex convex)
{
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
				//setFillColor(sf::Color::Red);
				return false;
			}
		}
	}
	//setFillColor(sf::Color::Green);
	return true;
}

Vector2f FloatConvex::intersects_static(FloatConvex convex)
{
	FloatConvex* poly1 = this;
	FloatConvex* poly2 = &convex;

	float overlap = INFINITY;

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

			overlap = std::min(std::min(max_r1, max_r2) - std::max(min_r1, min_r2), overlap);

			if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
			{
				setFillColor(sf::Color::Red);
				return Vector2f::zero();
			}
		}
	}

	Vector2f d = Vector2f(poly2->get_center().x - poly1->get_center().x, poly2->get_center().y - poly1->get_center().y);
	float s = sqrtf(d.x * d.x + d.y * d.y);

	poly1->set_position(Vector2f(poly1->get_center().x - overlap * d.x / s, poly1->get_center().y - overlap * d.x / s));

	setFillColor(sf::Color::Green);
	return Vector2f(poly1->get_center().x - overlap * d.x / s, poly1->get_center().y - overlap * d.x / s);
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

FloatConvex FloatConvex::sort_poly_model(FloatConvex poly)
{
	Vector2f og = poly.model[0];
	for (int k = 1; k < poly.model.size(); k++)
	{
		int j = k - 1;
		Vector2f temp = poly.model[k];
		while (j >= 0 &&
			FloatConvex::get_angle(og, poly.get_center(), poly.model[j]) >
			FloatConvex::get_angle(og, poly.get_center(), poly.model[k]))
		{
			poly.model[j + 1] = poly.model[j];
			j = j - 1;
		}
		poly.model[j + 1] = temp;
	}

	return poly;
}

float FloatConvex::get_angle(Vector2f a, Vector2f b, Vector2f c)
{
	float A = std::abs(Vector2f::sqr_distance(a, b));
	float B = std::abs(Vector2f::sqr_distance(b, c));
	float C = std::abs(Vector2f::sqr_distance(c, a));

	float angle = std::acos((A + B - C) / (2 * std::sqrt(A) * std::sqrt(B)));

	float x = Vector2f::cross_product(a - b, c - b);

	if (x < 0)
	{
		return angle;
	}
	else if (x == 0)
	{
		// DETERMINE IF ANGLE IS 0 OR 180
		bool is_zero = false;
		if ((a.x && c.x > 0 || a.x < 0 && c.x < 0) && (a.y && c.y > 0 || a.y < 0 && c.y < 0))
			return 0;

		return 180 * DEG2RAD;
		
	}
	else
	{
		return ((angle * RAD2DEG) + 180.0) * DEG2RAD;
	}
}

#pragma endregion


#pragma endregion





}