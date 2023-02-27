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
	
	float sqr_magnitude();
	
	static Vector2f down();
	static Vector2f up();
	static Vector2f left();
	static Vector2f right();
	static Vector2f zero();

	static float distance(sf::Vector2f a, sf::Vector2f b);
	static float sqr_distance(sf::Vector2f a, sf::Vector2f b);
	static float dot_product(sf::Vector2f a, sf::Vector2f b);
	static float cross_product(sf::Vector2f a, sf::Vector2f b);

	static bool equal(sf::Vector2f a, sf::Vector2f b);

	/// <summary>
	/// Returns a normalized copy of vector.
	/// </summary>
	Vector2f get_normalized();
	void normalize();
	/// <summary>
	/// Returns the normals of a line segment created between a and b.
	/// </summary>
	/// <returns></returns>
	static std::vector<Vector2f> get_normals(Vector2 a, Vector2f b);

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

	static Vector2i down();
	static Vector2i up();
	static Vector2i left();
	static Vector2i right();
	static Vector2i zero();

	static int distance(sf::Vector2i a, sf::Vector2i b);
	static int sqr_distance(sf::Vector2i a, sf::Vector2i b);
	static int dot_product(sf::Vector2i a, sf::Vector2i b);
	static bool equal(sf::Vector2i a, sf::Vector2i b);

	bool equals(sf::Vector2i a);

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;
};

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
	static FloatConvex circle(sf::Vector2f position, float radius, std::size_t point_count = 20);
	/// <summary>
	/// Creates a circle shape. Model positions are relative to position provided. 
	/// Additionally sets the shape to capsule, to attempt to optimize
	/// collision detection with other objects.
	/// </summary>
	static FloatConvex capsule(sf::Vector2f position, sf::Vector2f size, Direction direction,
		std::size_t point_count = 20);
	/// <summary>
	/// Creates a polygon shape. Model positions are relative to position provided.
	///  Will contain the least optimized collision detection, and should be avoided when possible.
	/// </summary>
	static FloatConvex polygon(sf::Vector2f position, std::vector<Vector2f> model);
	/// <summary>
	/// Creates a line. Line starts at start and ends at end.
	/// </summary>
	/// <param name="start"></param>
	/// <param name="end"></param>
	/// <returns></returns>
	static FloatConvex line(sf::Vector2f start, sf::Vector2f end, float thickness = 0);

	/// <summary>
	/// WORK IN PROGRESS...DOESNT WORK. 
	/// </summary>
	/// <param name="a"></param>
	/// <param name="b"></param>
	/// <returns></returns>
	static FloatConvex combine_shapes(FloatConvex a, FloatConvex b);

	void move(float x, float y);
	void set_position(sf::Vector2f position);
	Vector2f get_position();
	Vector2f get_center();

	bool intersects(FloatConvex convex);
	Vector2f intersects_static(FloatConvex convex);
	static bool intersection(FloatConvex a, FloatConvex b);


	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;




private:
	std::vector<Vector2f> model;
	Vector2f position;
	unsigned point_count;
	ShapeType shape_type = ShapeType::POLYGON;

	using ConvexShape::getPosition;
	using ConvexShape::setPosition;


	static FloatConvex sort_poly_model(FloatConvex poly);
	static float get_angle(Vector2f a, Vector2f b, Vector2f c);
};

}

