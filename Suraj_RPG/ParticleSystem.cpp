#include "pch.h"
#include "ParticleSystem.h"
namespace bm98
{
using namespace core;

ParticleSystem::ParticleSystem(unsigned count)
	:particles(count), vertices(sf::Points, count), life_time(3.f), emitter(0.f, 0.f)
{

}

ParticleSystem::ParticleSystem(unsigned count, float duration, float start_velocity, float end_velocity,
	float angle, float radius)
	: particles(count), vertices(sf::Points, count), life_time(duration), start_velocity(start_velocity),
	end_velocity(end_velocity), angle(angle), radius(radius), loop(true), start_on_creation(true),
	started(true), particles_per_sec(std::roundf(count / duration)), animation(false)
{

}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::set_emitter(sf::Vector2f position)
{
	emitter = position;
}

void ParticleSystem::start()
{
	started = true;
}

void ParticleSystem::update()
{
	for (std::size_t i = 0; i < particles.size(); ++i)
	{
		Particle& p = particles[i];
		p.life_time -= Time::Instance()->delta_time();

		if (p.life_time <= 0.f)
			reset_particle(i);

		vertices[i].position += p.velocity * (float)Time::Instance()->delta_time();
		float ratio = p.life_time / life_time;
		vertices[i].color.a = static_cast<sf::Uint8>(ratio * 255);

	}
}

void ParticleSystem::reset_particle(std::size_t index)
{
	float angle = (std::rand() % 360) * 3.14f / 180.f;
	float speed = (std::rand() % 50) + 50.f;
	particles[index].velocity = sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
	particles[index].life_time = ((std::rand() % 2000) + 1000) / 1000;

	// reset the position of the corresponding vertex
	vertices[index].position = emitter;
}

void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// apply the transform
	states.transform *= getTransform();

	// our particles don't use a texture
	states.texture = NULL;

	// draw the vertex array
	target.draw(vertices, states);
}
}