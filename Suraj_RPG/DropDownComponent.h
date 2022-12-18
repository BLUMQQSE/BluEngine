#pragma once
#include "Component.h"
#include "ButtonComponent.h"
namespace bm98
{

class DropDownComponent : public Component
{
public:
	DropDownComponent();
	DropDownComponent(float width, float height, std::vector<std::string> list,
		unsigned default_index, int char_size);
	virtual ~DropDownComponent();

	virtual void init() override;
	virtual void update() override;
	virtual void add_to_buffer(sf::View* view = nullptr) override;

	// Inherited via IData
	virtual Json::Value serialize_json() override;
	virtual void unserialize_json(Json::Value obj) override;
	

	void toggle_list(bool toggle);

	bool changed_selection();

	const bool mouse_in_bounds();

	// NOTE: this could be a point of failure for inconsistency,
	// tho with how i send in string vector to constructor I don't believe it should be
	void set_selected_index(int index);

	const short unsigned& get_selected_index() const;
	//const Button* get_selected_button() const;

private:
	sf::Font font;
	ButtonComponent* active_selection;
	std::vector<ButtonComponent*> list;
	bool show_list;
	bool selection_change;
	float width;
	float height;
};

}
