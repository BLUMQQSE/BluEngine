#pragma once
#include "EventSystem.h"

#include "../Timer.h"
#include "../Dialogue.h"
#include"../gui/Gui.h";

namespace bm98::core
{

class DialogueSystem : Listener
{
#define DEFAULT_CHARACTER_DURATION = .1f

public:
	// Returns instance of the DialogueSystem
	static DialogueSystem* Instance()
	{
		static DialogueSystem instance;
		return &instance;
	}

	void open_dialogue(Dialogue& dialogue);
	void close_dialogue();

	bool is_open() { return dialogue_open; }

private:
	DialogueSystem()
	{
		EventSystem::Instance()->subscribe(EventID::_SYSTEM_DIALOGUESYSTEM_UPDATE_, this);
	}
	~DialogueSystem() { shutdown(); }
	DialogueSystem(const DialogueSystem& rhs)
	{

	}
	DialogueSystem& operator=(const DialogueSystem& rhs) {}

	void update();
	void shutdown();

	void create_dialogue_panel();
	void update_dialogue_panel();
	void destroy_dialogue_panel();

	// Time spent since last character was displayed
	Dialogue* dialogue;
	Timer character_timer;
	bool dialogue_playing = false;
	bool dialogue_open = false;

	GUI::Panel* dialogue_panel;
	GUI::Panel* choices_panel;
	std::unique_ptr<GUI::RichText> statement;
	std::array<std::unique_ptr<GUI::RichText>, 4> choices;
	std::array<std::unique_ptr<GUI::Button>, 4> choice_buttons;


	// Inherited via Listener
	virtual void handle_event(Event* event) override;

};

}
