#pragma once
#include "IData.h"
namespace bm98
{

extern float PI;
extern float RAD2DEG;
extern float DEG2RAD;

/// <summary>
/// Class to wrap sf::Vector2f and add math functionality.
/// </summary>
class Vector2f : public sf::Vector2f, IData
{
public:
	Vector2f();
	Vector2f(float x, float y);
	Vector2f(sf::Vector2f vec);
	Vector2f(sf::Vector2i vec);

	float sqr_magnitude();
	float magnitude();

	std::string to_string();
	
	inline static Vector2f Down() { return Vector2f(0, 1); }
	inline static Vector2f Up() { return Vector2f(0, -1); }
	inline static Vector2f Left() { return Vector2f(-1, 0); }
	inline static Vector2f Right() { return Vector2f(1, 0); }
	inline static Vector2f Zero() { return Vector2f(0, 0); }
	inline static Vector2f Infinity() { return Vector2f(INFINITY, INFINITY); }

	static float Distance(sf::Vector2f a, sf::Vector2f b);
	static float SqrDistance(sf::Vector2f a, sf::Vector2f b);
	static float DotProduct(sf::Vector2f a, sf::Vector2f b);
	static float CrossProduct(sf::Vector2f a, sf::Vector2f b);

	/// <summary>
	/// Returns a normalized copy of vector.
	/// </summary>
	Vector2f get_normalized();
	static void Normalize(Vector2f& vec);

	/// <summary>
	/// Returns left normal. Should be used by FloatConvex to get normals
	/// exiting shape.
	/// </summary>
	inline static Vector2f GetLeft(Vector2f a) { return Vector2f(-a.y, a.x); }
	inline static Vector2f GetRight(Vector2f a) { return Vector2f(a.y, -a.x); }

	bool equals(sf::Vector2f a);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;

};

class Vector2i : public sf::Vector2i, IData
{
public:
	Vector2i();
	Vector2i(int x, int y);
	Vector2i(sf::Vector2i vec);

	int sqr_magnitude();

	inline static Vector2i Down() { return Vector2i(0, 1); }
	inline static Vector2i Up() { return Vector2i(0, -1); }
	inline static Vector2i Left() { return Vector2i(-1, 0); }
	inline static Vector2i Right() { return Vector2i(1, 0); }
	inline static Vector2i Zero() { return Vector2i(0,0); }
	inline static Vector2i Infinity() { return Vector2i(INFINITY, INFINITY); }

	static int Distance(sf::Vector2i a, sf::Vector2i b);
	static int SqrDistance(sf::Vector2i a, sf::Vector2i b);
	static int DotProduct(sf::Vector2i a, sf::Vector2i b);

	bool equals(sf::Vector2i a);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;
};

class ObjectIntersect;
/// <summary>
/// Class to wrap sf::ConvexShape and add math functionality.
/// </summary>
class FloatConvex : public sf::ConvexShape, IData
{
public:

#pragma region ShapeType
	enum class ShapeType
	{
		POLYGON,
		CIRCLE,
		CAPSULE,
		LINE
	};
	static std::vector<std::string> shapetype_to_vector()
	{
		return { "POLYGON", "CIRCLE", "CAPSULE", "LINE"};
	}
	static std::string shapetype_to_string(ShapeType type)
	{
		switch (type)
		{
		case ShapeType::CIRCLE:
			return "CIRCLE";
		case ShapeType::CAPSULE:
			return "CAPSULE";
		case ShapeType::POLYGON:
			return "POLYGON";
		case ShapeType::LINE:
			return "LINE";
		default:
			std::cout << "ERROR::MATH::shapetype_to_string::UNDEFINED DIRECTION: " << static_cast<int>(type) << "\n";
			return "null";
		}
	}
	static ShapeType string_to_shapetype(std::string type)
	{
		if (type == "CIRCLE")
			return ShapeType::CIRCLE;
		if (type == "CAPSULE")
			return ShapeType::CAPSULE;
		if (type == "POLYGON")
			return ShapeType::POLYGON;
		if (type == "LINE")
			return ShapeType::LINE;

		std::cout << "ERROR::MATH::string_to_shapetype::UNDEFINED STRING: " << type << "\n";
		return ShapeType::POLYGON;
	}
#pragma endregion
#pragma region Direction
	enum class Direction
	{
		VERTICAL,
		HORIZONTAL
	};
	static std::vector<std::string> direction_to_vector()
	{
		return { "VERTICAL", "HORIZONTAL" };
	}
	static std::string directione_to_string(Direction dir)
	{
		switch (dir)
		{
		case Direction::VERTICAL:
			return "VERTICAL";
		case Direction::HORIZONTAL:
			return "HORIZONTAL";
		default:
			std::cout << "ERROR::MATH::direction_to_string::UNDEFINED DIRECTION: " << static_cast<int>(dir) << "\n";
			return "null";
		}
	}
	static Direction string_to_direction(std::string dir)
	{
		if (dir == "VERTICAL")
			return Direction::VERTICAL;
		if (dir == "HORIZONTAL")
			return Direction::HORIZONTAL;

		std::cout << "ERROR::MATH::string_to_direction::UNDEFINED STRING: " << dir << "\n";
		return Direction::VERTICAL;
	}
#pragma endregion

	/// <summary>
	/// Creates an empty convex shape.
	/// </summary>
	FloatConvex();

	/// <summary>
	/// Creates a circle shape.  Model positions are relative to position provided.
	/// Additionally sets the shape to circle, to attempt to optimize
	/// collision detection with other objects.
	/// </summary>
	static FloatConvex Circle(sf::Vector2f position, float radius, std::size_t point_count = 20);
	/// <summary>
	/// Creates a circle shape. Model positions are relative to position provided. 
	/// Additionally sets the shape to capsule, to attempt to optimize
	/// collision detection with other objects.
	/// </summary>
	static FloatConvex Capsule(sf::Vector2f position, sf::Vector2f size, Direction direction,
		std::size_t point_count = 20);
	/// <summary>
	/// Creates a polygon shape. Model positions are relative to position provided.
	///  Will contain the least optimized collision detection, and should be avoided when possible.
	/// </summary>
	static FloatConvex Polygon(sf::Vector2f position, std::vector<Vector2f> model);
	
	static FloatConvex Rectangle(sf::Vector2f position, sf::Vector2f size);
	/// <summary>
	/// Creates a line. Line starts at start and ends at end.
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static FloatConvex Line(sf::Vector2f start, sf::Vector2f end, float thickness = 0);

	/// <summary>
	/// WORK IN PROGRESS...DOESNT WORK. 
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static FloatConvex CombineShapes(FloatConvex a, FloatConvex b);

	void init();

	bool contains_point(Vector2f position);

	void move(float x, float y);
	void set_position(sf::Vector2f position);
	void set_rotation(float new_rot);
	void rotate(float rot_offset);
	inline Vector2f get_position() { return position; }
	Vector2f get_center();
	Vector2f get_model_center();
	inline std::vector<Vector2f> get_model() { return model; }
	inline ShapeType get_shape_type() { return shape_type; }

	Vector2f get_closest_point(Vector2f pos);

	/// <summary>
	/// Determines if two shapes overlap.
	/// </summary>
	/// <returns>The minimum penetration vector between the two shapes. This vector is
	/// a.position - b.position. If no collision, returns Vector2f::infinity().</returns>
	static ObjectIntersect Intersection(FloatConvex a, FloatConvex b);


	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;



private:
	std::vector<Vector2f> model;
	Vector2f position;
	unsigned point_count;
	ShapeType shape_type = ShapeType::POLYGON;
	float rotation = 0;

	using ConvexShape::getPosition;
	using ConvexShape::setPosition;

	static bool PreliminaryCircleCheck(FloatConvex a, FloatConvex b);

	static void SortPolyModel(FloatConvex& poly);
	static float GetAngle(Vector2f a, Vector2f b, Vector2f c);
	static void ProjectVertices(FloatConvex a, Vector2f axis,
		float& min, float& max);


};


class Intersect
{
public:

	bool collision_exists = false;
	Vector2f penetration_vector = Vector2f::Infinity();

	virtual void calculate_contacts(Vector2f first = Vector2f::Zero(),
									Vector2f second = Vector2f::Zero()) {}

	std::vector<Vector2f> get_contact_points()
	{
		if (contact_points.size() == 0)
			calculate_contacts();
		return contact_points;
	}

	Vector2f get_contact_center()
	{
		if (contact_center == Vector2f::Infinity())
			calculate_contacts();
		return contact_center;
	}

protected:
	std::vector<Vector2f> contact_points;
	// Average of all points in contact
	Vector2f contact_center = Vector2f::Infinity();

};

class ObjectIntersect : public Intersect
{
public:
	FloatConvex convex_1;
	FloatConvex convex_2;

	ObjectIntersect()
	{

	}

	ObjectIntersect(Intersect i)
	{
		collision_exists = i.collision_exists;
		penetration_vector = i.penetration_vector;
		contact_center = i.get_contact_center();
		contact_points = i.get_contact_points();
	}

	void calculate_contacts(Vector2f first = Vector2f::Zero(), 
							Vector2f second = Vector2f::Zero()) override
	{
		if (contact_points.size() > 0)
			return;

		convex_1.move(first.x, first.y);
		convex_2.move(second.x, second.y);

		for (int i = 0; i < convex_1.getPointCount(); i++)
		{
			if (convex_2.contains_point(convex_1.getPoint(i)))
				contact_points.push_back(convex_1.getPoint(i));
		}
		for (int i = 0; i < convex_2.getPointCount(); i++)
		{
			if (convex_1.contains_point(convex_2.getPoint(i)))
				contact_points.push_back(convex_2.getPoint(i));
		}

		for (int i = 0; i < contact_points.size(); i++)
			contact_center += contact_points[i];

		if (contact_points.size() > 0)
		{
			contact_center.x /= contact_points.size();
		}
	}


};

class TilemapIntersect : public Intersect
{
public:

	TilemapIntersect()
	{

	}
	TilemapIntersect(Intersect i)
	{
		collision_exists = i.collision_exists;
		penetration_vector = i.penetration_vector;
		contact_center = i.get_contact_center();
		contact_points = i.get_contact_points();
	}
	TilemapIntersect(ObjectIntersect o)
	{
		convex_1.push_back(o.convex_1);
		convex_2.push_back(o.convex_2);
		collision_exists = o.collision_exists;
		penetration_vector = o.penetration_vector;
		contact_center = o.get_contact_center();
		contact_points = o.get_contact_points();
	}

	std::vector<FloatConvex> convex_1;
	std::vector<FloatConvex> convex_2;
	virtual void calculate_contacts(Vector2f first = Vector2f::Zero(),
									Vector2f second = Vector2f::Zero())
	{
		for(auto& c1 : convex_1)
			c1.move(first.x, first.y);
		for(auto& c2 : convex_2)
			c2.move(second.x, second.y);

		for (auto& c1 : convex_1)
		{
			for (int i = 0; i < c1.getPointCount(); i++)
			{
				for (auto& c2 : convex_2)
				{
					if (c2.contains_point(c1.getPoint(i)))
						contact_points.push_back(c1.getPoint(i));
				}
			}
		}
		for (auto& c2 : convex_2)
		{
			for (int i = 0; i < c2.getPointCount(); i++)
			{
				for (auto& c1 : convex_1)
				{
					if (c1.contains_point(c2.getPoint(i)))
						contact_points.push_back(c2.getPoint(i));
				}
			}
		}
		for (int i = 0; i < contact_points.size(); i++)
			contact_center += contact_points[i];

		if (contact_points.size() > 0)
		{
			contact_center.x /= contact_points.size();
		}
	}
};


}

