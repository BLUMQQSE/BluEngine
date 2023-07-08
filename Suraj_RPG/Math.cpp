#include "pch.h"
#include "Math.h"
#include "core/Renderer.h"

namespace bm98
{

float PI = 3.14159265358979323846;
float RAD2DEG = 180.0 / PI;
float DEG2RAD = PI / 180.0;

#pragma region Vector3f

Vector3f::Vector3f()
{
	x = 0.f;
	y = 0.f;
	z = 0.f;
}

Vector3f::Vector3f(float x, float y, float z) 
{
	this->x = x;
	this->y = y;
	this->z = z;
}

Vector3f::Vector3f(sf::Vector3f vec)
{
	this->x = x;
	this->y = y;
}

Vector3f::Vector3f(sf::Vector2f vec)
{
	this->x = vec.x;
	this->y = vec.y;
	this->z = 0.f;
}

Json::Value Vector3f::serialize_json()
{
	Json::Value obj;

	obj["x"] = x;
	obj["y"] = y;
	obj["z"] = z;

	return obj;
}

void Vector3f::unserialize_json(Json::Value obj)
{
	x = obj["x"].asFloat();
	y = obj["y"].asFloat();
	z = obj["z"].asFloat();
}

#pragma endregion


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

Vector2f::Vector2f(sf::Vector2i vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

Vector2f::Vector2f(sf::Vector3f vec)
{
	this->x = vec.x;
	this->y = vec.y;
}

float Vector2f::sqr_magnitude()
{
	return std::pow(x, 2) + std::pow(y, 2);
}

float Vector2f::magnitude()
{
	return std::sqrt(sqr_magnitude());
}

std::string Vector2f::to_string()
{
	return std::string("(" + std::to_string(x) + ", " + std::to_string(y) + ")");
}

float Vector2f::Distance(sf::Vector2f a, sf::Vector2f b)
{
	return std::sqrt(std::pow(b.x - a.x, 2) + std::pow(b.y - a.y, 2));
}

float Vector2f::SqrDistance(sf::Vector2f a, sf::Vector2f b)
{
	return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

float Vector2f::DotProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.x + a.y * b.y;
}

float Vector2f::CrossProduct(sf::Vector2f a, sf::Vector2f b)
{
	return a.x * b.y - a.y * b.x;
}

Vector2f Vector2f::get_normalized()
{
	if (sqr_magnitude() != 0)
	{
		return Vector2f(x / std::sqrt(sqr_magnitude()), y / std::sqrt(sqr_magnitude()));
	}
	return Vector2f(x, y);
}

void Vector2f::Normalize(Vector2f& vec)
{
	vec = vec.get_normalized();
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

int Vector2i::Distance(sf::Vector2i a, sf::Vector2i b)
{
	return std::sqrt(std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2));
}

int Vector2i::SqrDistance(sf::Vector2i a, sf::Vector2i b)
{
	return std::pow(a.x - b.x, 2) + std::pow(a.y - b.y, 2);
}

int Vector2i::DotProduct(sf::Vector2i a, sf::Vector2i b)
{
	return a.x * b.x + a.y * b.y;
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

FloatConvex FloatConvex::Circle(sf::Vector2f position, float radius, std::size_t point_count)
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

FloatConvex FloatConvex::Capsule(sf::Vector2f position, sf::Vector2f size, Direction dir, std::size_t point_count)
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

FloatConvex FloatConvex::Polygon(sf::Vector2f position, std::vector<Vector2f> model)
{
	FloatConvex convex;

	convex.shape_type = ShapeType::POLYGON;

	convex.setPointCount(model.size());
	convex.point_count = model.size();
	convex.model = model;
	convex.set_position(position);

	return convex;
}

FloatConvex FloatConvex::Rectangle(sf::Vector2f position, sf::Vector2f size)
{
	return FloatConvex::Polygon(position,
								{ Vector2f(0,0), Vector2f(size.x, 0),
								Vector2f(size.x, size.y), Vector2f(0, size.y) });
}

FloatConvex FloatConvex::Line(sf::Vector2f start, sf::Vector2f end, float thickness)
{
	FloatConvex convex;

	Vector2f normal = Vector2f::GetLeft(end - start);

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

FloatConvex FloatConvex::CombineShapes(FloatConvex a, FloatConvex b)
{
	FloatConvex convex;

	if (a.position.x < b.position.x)
		convex.set_position(a.position);
	else
		convex.set_position(b.position);

	convex.model.insert(convex.model.end(), a.model.begin(), a.model.end());
	convex.model.insert(convex.model.end(), b.model.begin(), b.model.end());

	SortPolyModel(convex);

	// remove duplicates
	//std::unique(convex.model.begin(), convex.model.end());

	convex.setPointCount(convex.model.size());
	convex.point_count = convex.model.size();

	convex.shape_type = ShapeType::POLYGON;

	Vector2f center = convex.get_center();

	return convex;
}

void FloatConvex::init()
{
	setPointCount(point_count);
	model.resize(point_count);
	set_position(position);
}

bool FloatConvex::contains_point(Vector2f pos)
{
	int p = 0;
	int n = 0;
	for (int i = 0; i < model.size(); i++)
	{
		int ii = (i + 1) % model.size();
		Vector2f a = model[i] + position;
		Vector2f b = model[ii] + position;
		if (a == pos || b == pos)
			return true;

		Vector2f ab = b - a;
		Vector2f ap = pos - a;

		if (Vector2f::CrossProduct(ab, ap) > 0)
			p++;
		else
			n++;
		if (p > 0 && n > 0)
			return false;

		//if ((b.x - a.x) * (position.y - a.y) - (b.y - a.y) * (position.x - a.x) == 0)
		//	return false;
	}
	return true;
	//FloatConvex point = FloatConvex::Polygon(position, { Vector2f(0,0), Vector2f(1, 0), Vector2f(0.5f, 1) });
	//return FloatConvex::Intersection(*this, point).collision_exists;
}

LineIntersect FloatConvex::line_intersects(Vector2f line_start, Vector2f line_end)
{
	LineIntersect inter;
	for (int i = 0; i < model.size(); i++)
	{
		int ii = (i + 1) % model.size();
		Vector2f check = solve_line_intersection(model[i] + position, model[ii] + position, line_start, line_end);
		if (check != Vector2f::Infinity())
			inter.add_contact_point(check, line_start);
	}

	return inter;
}

Vector2f FloatConvex::solve_line_intersection(Vector2f a, Vector2f b, Vector2f c, Vector2f d)
{
	Vector2f ab = b - a;
	Vector2f cd = d - c;
	float t1 = 0, t2 = 0;

	float ab_cross_cd = Vector2f::CrossProduct(ab, cd);

	if (ab_cross_cd == 0)
		return Vector2f::Infinity();
	else
	{
		Vector2f ac = c - a;
		t1 = Vector2f::CrossProduct(ac, cd) / ab_cross_cd;
		t2 = -Vector2f::CrossProduct(ab, ac) / ab_cross_cd;
		ab *= t1;

		if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1)
			return a + ab;
		return Vector2f::Infinity();
	}

}

void FloatConvex::move(float x, float y)
{
	set_position(Vector2f(position.x + x, position.y + y));
}

void FloatConvex::set_position(sf::Vector2f position)
{
	this->position = position;
	for (std::size_t i = 0; i != model.size(); i++)
	{
		setPoint(i, model[i] + position);
	}

}

void FloatConvex::set_rotation(float new_rot)
{
	float dif = new_rot - this->rotation;
	rotate(dif);
}

void FloatConvex::rotate(float rot_offset)
{
	if (rotation + rot_offset >= 360)
	{
		float dif = rotation + rot_offset - 360;
		rotation = dif;
	}
	else if (rotation + rot_offset < 0)
	{
		float dif = rotation + rot_offset;
		rotation = 360 - std::abs(rotation + rot_offset);
	}
	else
		rotation += rot_offset;

	for (int i = 0; i < getPointCount(); i++)
	{
		Vector2f new_point;

		new_point.x = (std::cos(rot_offset) * getPoint(i).x) - (std::sin(rot_offset) + getPoint(i).y);
		new_point.y = (std::sin(rot_offset) * getPoint(i).x) + (std::cos(rot_offset) * getPoint(i).y);

		setPoint(i, new_point);
	}

	// TODO: implement rotation

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

Vector2f FloatConvex::get_model_center()
{
	Vector2f c;

	for (int p = 0; p < model.size(); p++)
	{
		c += model[p];
	}
	c.x /= model.size();
	c.y /= model.size();

	return c;
}

Vector2f FloatConvex::get_closest_point(Vector2f pos)
{
	Vector2f closest;

	closest = position + model[0];
	for (int i = 1; i < model.size(); i++)
	{
		if (Vector2f::SqrDistance(position + model[i], pos) <
			Vector2f::SqrDistance(closest, pos))
		{
			closest = position + model[i];
		}
	}

	return closest;
}

ObjectIntersect FloatConvex::Intersection(FloatConvex a, FloatConvex b, Vector2f a_movement,
										  Vector2f b_movement)
{
	ObjectIntersect intersect{ a,b };

	if (a.get_shape_type() == ShapeType::CIRCLE && b.get_shape_type() == ShapeType::CIRCLE)
	{
		if (PreliminaryCircleCheck(a, b) == false)
		{
			return intersect;
		}
	}

	Vector2f normal = Vector2f::Infinity();
	float depth = INFINITY;

	for (int i = 0; i < a.getPointCount(); i++)
	{
		Vector2f va = a.getPoint(i);
		Vector2f vb = a.getPoint((i + 1) % a.getPointCount());

		Vector2f edge = vb - va;
		Vector2f axis = Vector2f(-edge.y, edge.x);

		float minA, maxA, minB, maxB;

		ProjectVertices(a, axis, minA, maxA);
		ProjectVertices(b, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return intersect;

		float axisDepth = std::min(maxB - minA, maxA - minB);
		if (axisDepth < depth)
		{
			//intersect.detection_point = 
			depth = axisDepth;
			normal = axis;
		}

	}

	for (int i = 0; i < b.getPointCount(); i++)
	{
		Vector2f va = b.getPoint(i);
		Vector2f vb = b.getPoint((i + 1) % b.getPointCount());

		Vector2f edge = vb - va;
		Vector2f axis = Vector2f(-edge.y, edge.x);

		float minA, maxA, minB, maxB;

		ProjectVertices(a, axis, minA, maxA);
		ProjectVertices(b, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA)
			return intersect;

		float axisDepth = std::min(maxB - minA, maxA - minB);
		if (axisDepth < depth)
		{
			//intersect.detection_point = 
			depth = axisDepth;
			normal = axis;
		}

	}

	depth /= normal.magnitude();
	Vector2f::Normalize(normal);

	Vector2f centerA = a.get_center();
	Vector2f centerB = b.get_center();

	Vector2f dir = centerB - centerA;

	if (Vector2f::DotProduct(dir, normal) > 0.f)
		normal = -normal;

	// subtract 1 from depth so that after collision is resolved can still detect the shapes are touching
	intersect.penetration_vector = (depth - 1) * normal;
	intersect.collision_exists = true;

	//intersect.calculate_contacts(a_movement, b_movement);

	return intersect;

}

#pragma region IDATA


Json::Value FloatConvex::serialize_json()
{
	Json::Value obj;

	obj["point-count"] = point_count;
	obj["position"] = position.serialize_json();
	obj["shape-type"] = shapetype_to_string(shape_type);
	obj["rotation"] = rotation;

	for (auto& p : model)
	{
		obj["model"].append(p.serialize_json());
	}

	return obj;
}

void FloatConvex::unserialize_json(Json::Value obj)
{
	point_count = obj["point-count"].asUInt64();
	setPointCount(point_count);
	position.unserialize_json(obj["position"]);
	shape_type = string_to_shapetype(obj["shape-type"].asString());
	rotation = obj["rotation"].asFloat();

	model.resize(point_count);
	int x = 0;
	for (Json::Value point : obj["model"])
	{
		model[x].unserialize_json(point);
		x++;
	}

	set_position(position);
}

bool FloatConvex::PreliminaryCircleCheck(FloatConvex a, FloatConvex b)
{
	Vector2f a_center = a.get_center();
	Vector2f b_center = b.get_center();

	float a_radius = Vector2f::Distance(a.getPoint(0), a_center);
	float b_radius = Vector2f::Distance(b.getPoint(0), b_center);

	float dist_between_centers = Vector2f::Distance(a_center, b_center);

	if (dist_between_centers < a_radius + b_radius)
		return true;

	return false;

}

void FloatConvex::SortPolyModel(FloatConvex& poly)
{
	Vector2f og = poly.model[0];
	for (int k = 1; k < poly.model.size(); k++)
	{
		int j = k - 1;
		Vector2f temp = poly.model[k];
		while (j >= 0 &&
			   FloatConvex::GetAngle(og, poly.get_center(), poly.model[j]) >
			   FloatConvex::GetAngle(og, poly.get_center(), poly.model[k]))
		{
			poly.model[j + 1] = poly.model[j];
			j = j - 1;
		}
		poly.model[j + 1] = temp;
	}
}

float FloatConvex::GetAngle(Vector2f a, Vector2f b, Vector2f c)
{
	float A = std::abs(Vector2f::SqrDistance(a, b));
	float B = std::abs(Vector2f::SqrDistance(b, c));
	float C = std::abs(Vector2f::SqrDistance(c, a));

	float angle = std::acos((A + B - C) / (2 * std::sqrt(A) * std::sqrt(B)));

	float x = Vector2f::CrossProduct(a - b, c - b);

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

void FloatConvex::ProjectVertices(FloatConvex a, Vector2f axis, float& min, float& max)
{
	min = INFINITY;
	max = -INFINITY;

	for (int i = 0; i < a.getPointCount(); i++)
	{
		Vector2f v = a.getPoint(i);
		float proj = Vector2f::DotProduct(v, axis);

		if (proj < min) { min = proj; }
		if (proj > max) { max = proj; }

	}
}

#pragma endregion


#pragma endregion



}