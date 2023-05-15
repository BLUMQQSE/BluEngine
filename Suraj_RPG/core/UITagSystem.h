#pragma once

#include "EventSystem.h"
#include "SceneManager.h"

#include "../IRenderable.h"
#include "../Timer.h"
#include "../Gui.h"

namespace bm98
{

class UITag : public IRenderable
{
public:
	// Could use a tag type to define internally its behaviour,
	// then any tag created will need to define its tag type
	enum class Action
	{
		NO_ACTION,
		DISAPPEAR,
		/// <summary> Reqiures passing in a std::pair(Vector2f move_speed, 
		/// float fade_speed)
		/// pointer. </summary>
		MOVE_AND_FADE,
		/// <summary> Reqiures passing in a std::pair(Vector2f explode_speed, 
		/// float fade_speed)
		/// pointer. </summary>
		EXPLODE_AND_FADE,
		/// <summary> Requires passing in a Vector2f. </summary>
		FOLLOW,
		/// <summary> Requires passing in a Vector2f. </summary>
		FADE,
		/// <summary> Requires passing in a Vector2f. </summary>
		MOVE,

		// STILL TO BE PROGRAMMED
		EXPLODE,
		 

	};

	struct ActionSegment
	{
		ActionSegment(){}
		ActionSegment(Action action, float start_time, void* value)
		{
			this->action = action;
			this->start_time = start_time;
			this->value = value;
		}
		Action action = Action::NO_ACTION;
		float start_time = 0;
		void* value = nullptr;
	};

	UITag();
	virtual ~UITag() {}

	/// <summary>
	/// Moves and fades the renderable object. Returns false if the renderable has reached
	/// it's final stage.
	/// </summary>
	/// <returns></returns>
	void update();

	virtual void no_action(const ActionSegment& action) = 0;
	virtual void disappear(const ActionSegment& action) = 0;
	void move_and_fade(const ActionSegment& action, 
					   Vector2f move_speed, float fade_speed)
	{
		if (fade(action, fade_speed))
		{
			move(action, move_speed);
		}
	}
	void explode_and_fade(const ActionSegment& action,
						float explode_speed, float fade_speed)
	{
		if (fade(action, fade_speed))
		{
			explode(action, explode_speed);
		}
	}
	virtual void move(const ActionSegment& action, Vector2f move_speed) = 0;
	virtual bool fade(const ActionSegment& action, float fade_speed) = 0;
	virtual void follow(const ActionSegment& action) = 0;
	virtual void explode(const ActionSegment& action, float explode_speed) = 0;

	/// <summary>
	/// Add an action for the UITag. Actions must be added in sequential order.
	/// </summary>
	void add_action(Action action, float start_time, void* value);

	const float get_duration() const { return duration; }
	const float get_elapsed_time() { return timer.get_elapsed_time().asSeconds(); }
	const void start_timer() { timer.restart(); }

protected:
	float duration;
	Timer timer;
	float alpha = 255;

	std::list<ActionSegment> actions;

};

class ImageUITag : public UITag
{
public:
	ImageUITag(Vector2f position, Vector2f size, sf::Texture* texture, float duration, 
			   sf::View* view = core::SceneManager::Instance()->get_active_scene_view());
	virtual ~ImageUITag();

	virtual void no_action(const ActionSegment& action) override;
	virtual void disappear(const ActionSegment& action) override;
	virtual void move(const ActionSegment& action, Vector2f move_speed) override;
	virtual bool fade(const ActionSegment& action, float fade_speed) override;
	virtual void follow(const ActionSegment& action) override;
	virtual void explode(const ActionSegment& action, float explode_speed) override;

private:
	FloatConvex image;
	// if want to implement explode:
	// can add vector<FloatConvex> image_fragments
};

class TextUITag : public UITag
{
public:
	TextUITag(Vector2f position, std::string text, unsigned font_size, float duration,
			  sf::View* view = core::SceneManager::Instance()->get_active_scene_view());
	virtual ~TextUITag();

	virtual void set_view(sf::View* view) override;

	virtual void no_action(const ActionSegment& action) override;
	virtual void disappear(const ActionSegment& action) override;
	virtual void move(const ActionSegment& action, Vector2f move_speed) override;
	virtual bool fade(const ActionSegment& action, float fade_speed) override;
	virtual void follow(const ActionSegment& action) override;
	virtual void explode(const ActionSegment& action, float explode_speed) override;
private:
	GUI::RichText* rt;
	sf::Color fill_color;
	sf::Color outline_color;
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

	TextUITag* create_text_tag(Vector2f position, std::string text, unsigned font_size, float duration,
							   sf::View* view = core::SceneManager::Instance()->get_active_scene_view());
	ImageUITag* create_image_tag(Vector2f position, Vector2f size, sf::Texture* texture, float duration,
								 sf::View* view = core::SceneManager::Instance()->get_active_scene_view());

	/// <summary>
	/// Add a tag to the system. If creating a UITag independently, this will need called,
	/// However using the system 'create_tag' functions will handle this automatically.
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
	/// <summary>
	/// Destroys all tags in the system.
	/// </summary>
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
