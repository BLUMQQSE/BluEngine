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
	float magnitude();

	std::string to_string();
	
	static Vector2f Down();
	static Vector2f Up();
	static Vector2f Left();
	static Vector2f Right();
	static Vector2f Zero();
	static Vector2f Infinity();

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
	static Vector2f GetLeft (Vector2f a);
	static Vector2f GetRight(Vector2f a);

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

	static Vector2i Down();
	static Vector2i Up();
	static Vector2i Left();
	static Vector2i Right();
	static Vector2i Zero();

	static int Distance(sf::Vector2i a, sf::Vector2i b);
	static int SqrDistance(sf::Vector2i a, sf::Vector2i b);
	static int DotProduct(sf::Vector2i a, sf::Vector2i b);

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

	void move(float x, float y);
	void set_position(sf::Vector2f position);
	void set_rotation(float new_rot);
	void rotate(float rot_offset);
	Vector2f get_position();
	Vector2f get_center();
	Vector2f get_model_center();
	std::vector<Vector2f> get_model();
	ShapeType get_shape_type();

	/// <summary>
	/// Determines if two shapes overlap.
	/// </summary>
	/// <returns>The minimum penetration vector between the two shapes. This vector is
	/// a.position - b.position. If no collision, returns Vector2f::infinity().</returns>
	static Vector2f Intersection(FloatConvex a, FloatConvex b);


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


	static void SortPolyModel(FloatConvex& poly);
	static float GetAngle(Vector2f a, Vector2f b, Vector2f c);
	static void ProjectVertices(FloatConvex a, Vector2f axis,
		float& min, float& max);


};

}

