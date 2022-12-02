#pragma once
#include "Time.h"
namespace bm98
{
class ParticleSystem : public sf::Drawable, sf::Transformable
{
public:
	ParticleSystem(unsigned count);
	ParticleSystem(unsigned count, float duration, float start_velocity, float end_velocity,
		float angle, float radius);
	virtual ~ParticleSystem();

	void set_emitter(sf::Vector2f position);
	void start();

	void update();

private:
	struct Particle
	{
		sf::Vector2f velocity;
		float life_time;
	};

	std::vector<Particle> particles;
	sf::VertexArray vertices;
	float life_time;
	sf::Vector2f emitter;

	float start_size;
	float end_size;
	float start_velocity;
	float end_velocity;
	float duration;
	int particles_per_sec;

	float angle;
	float radius;

	bool loop;
	bool animation;
	//AnimatedSprite sprite;
	bool start_on_creation;
	bool started;

	void reset_particle(std::size_t index);
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};
}

