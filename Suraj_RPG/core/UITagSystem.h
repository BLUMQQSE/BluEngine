#pragma once
#include "../IRenderable.h"
#include "EventSystem.h"
#include "../Timer.h"
#include "SceneManager.h"

namespace bm98
{

class UITag : public IRenderable
{
public:

	UITag() {}
	virtual ~UITag() {}

	/// <summary>
	/// Moves and fades the renderable object. Returns false if the renderable has reached
	/// it's final stage.
	/// </summary>
	/// <returns></returns>
	virtual bool move_and_fade() = 0;

	const float get_duration() const { return duration; }
	const float get_movement_speed() const { return movement_speed; }
	const float get_elapsed_time() { return timer.get_elapsed_time().asSeconds(); }
	const void start_timer() { timer.restart(); }

protected:
	float duration;
	float movement_speed;
	Timer timer;


};

class ImageUITag : public UITag
{
public:
	ImageUITag(Vector2f position, Vector2f size, sf::Texture* texture, float duration, 
			   sf::View* view = core::SceneManager::Instance()->get_active_scene_view());
	virtual ~ImageUITag();

	virtual bool move_and_fade() override;
private:
	FloatConvex image;
};

class TextUITag : public UITag
{
public:
	TextUITag(Vector2f position, std::string text, unsigned font_size, float duration,
			  sf::View* view = core::SceneManager::Instance()->get_active_scene_view());
	virtual ~TextUITag();

	virtual bool move_and_fade() override;
private:
	sf::Text text;
};

namespace core
{

class UITagSystem : public Listener
{
public:
	// Returns instance of the UITagSystem
	static UITagSystem* Instance()
	{
		static UITagSystem instance;
		return &instance;
	}


	/// <summary>
	/// Add a tag to the system.
	/// </summary>
	void add(UITag* tag);
	/// <summary>
	/// Removes and destroys a tag object found within the system.
	/// Destruction will automatically be handled by the system, but this function
	/// can be called at any time.
	/// </summary>
	void destroy(UITag* tag);

private:
	UITagSystem();
	~UITagSystem() { shutdown(); }
	UITagSystem(const UITagSystem& rhs)
	{

	}
	UITagSystem& operator=(const UITagSystem& rhs) {}


	void update();

	void clear();
	/// <summary>
	/// Destroys all tags in the system.
	/// </summary>
	void shutdown();

	std::list<UITag*> tags;

	Sorting::Layer layer = Sorting::Layer::UI;
	char z = 3;


	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}

}
