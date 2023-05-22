#include "../pch.h"
#include "DialogueSystem.h"

#include "ResourceManager.h"
#include "Renderer.h"

#include "Input.h"

namespace bm98::core
{


void DialogueSystem::open_dialogue(Dialogue& dialogue)
{
	if (dialogue_open)
		return;

	dialogue_open = true;
	this->dialogue = &dialogue;

	dialogue_playing = true;

	//statement = new GUI::RichText();
	statement = std::make_unique<GUI::RichText>();
	for (std::size_t i = 0; i < 4; i++)
	{
		choices[i] = std::make_unique<GUI::RichText>();
		choice_buttons[i] = std::make_unique<GUI::Button>();
	}
	create_dialogue_panel();
	
	this->dialogue->find_entry_statement();

	bool display_choices = false;

	if (this->dialogue->statement_ended())	// Display Choices if finished statement
		display_choices = true;

	statement->reinit(dialogue_panel->get_position() + Vector2f(50, 50), this->dialogue->get_text(), 26, 200);
	statement->set_render(true);

	if (display_choices)
	{
		choices_panel->set_render(true);
		int choice_count = this->dialogue->get_choices().size();
		for (int i = 0; i < choice_count; i++)
		{
			choice_buttons[i]->set_render(true);

			choice_buttons[i]->reinit(choice_buttons[i]->get_position().x, choices_panel->get_position().y + i * choices_panel->get_height() / choice_count, choices_panel->get_width(), choices_panel->get_height() / choice_count,
									  &ResourceManager::Instance()->get_font("calibri-regular.ttf"), this->dialogue->get_choices()[i], 20,
									  sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
									  sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));
		}
	}
	else
	{
		choices_panel->set_render(false);
	}
}

void DialogueSystem::close_dialogue()
{
	destroy_dialogue_panel();

	dialogue_open = false;
	dialogue_playing = false;

}

void DialogueSystem::update()
{
	if (!dialogue_open)
		return;

	for (auto& c : choice_buttons)
		c->update();
	
	if (Input::Instance()->get_mouse_down(Input::Mouse::LEFT) || Input::Instance()->get_action_down("ROLL"))
	{
		if (!dialogue->can_continue())
		{
			close_dialogue();
			return;
		}

		update_dialogue_panel();
		
	}
}

void DialogueSystem::shutdown()
{
	close_dialogue();
}

void DialogueSystem::create_dialogue_panel()
{
	dialogue_panel = new GUI::Panel(0, Renderer::Instance()->get_window_size().y - 200, Renderer::Instance()->get_window_size().x, 200);
	dialogue_panel->add_element("statement", statement.get());
	statement->set_render(false);
	//choices_panel = new GUI::Panel(Renderer::Instance()->get_window_size().x - 150, dialogue_panel->get_position().y + 200, 150, 200);
	choices_panel = new GUI::Panel(500, 300, 200, 200);
	int choice = 0;
	for (auto& b : choice_buttons)
	{
		choices_panel->add_element("choice"+std::to_string(choice), b.get());
		choice++;
	}
}

void DialogueSystem::update_dialogue_panel()
{
	bool display_choices = false;
	if (!dialogue->can_continue())
	{
		close_dialogue();
		return;
	}
	if (!dialogue->statement_ended())		// Just progress statement
	{
		dialogue->next();

		if (dialogue->statement_ended())	// Display Choices if finished statement
			display_choices = true;
	
	}
	else									// Progress dialogue based on statement
	{
		int choice = 0;
		for (auto& c : choice_buttons)
		{
			if (c->is_pressed())
			{
				dialogue->next(choice);
				break;
			}
			choice++;
		}
	}

	// TODO: Display next_string as new statement
	statement->reinit(dialogue_panel->get_position() + Vector2f(50, 50), dialogue->get_text(), 26, 100);
	statement->set_render(true);
	// TODO: If display_choices set choices to render and update their text and size

	if (display_choices)
	{
		choices_panel->set_render(true);
		int choice_count = dialogue->get_choices().size();
		for (int i = 0; i < choice_count; i++)
		{
			choice_buttons[i]->set_render(true);

			choice_buttons[i]->reinit(choice_buttons[i]->get_position().x, choices_panel->get_position().y + i * choices_panel->get_height() / choice_count, choices_panel->get_width(), choices_panel->get_height() / choice_count,
									  &ResourceManager::Instance()->get_font("calibri-regular.ttf"), dialogue->get_choices()[i], 20,
									  sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 200), sf::Color(255, 255, 255, 150),
									  sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255), sf::Color(0, 0, 200, 255));
		}

		for (int i = choice_count; i < 4; i++)
			choice_buttons[i]->set_render(false);
	}
	else
	{
		choices_panel->set_render(false);
	}

}

void DialogueSystem::destroy_dialogue_panel()
{
	if (dialogue_open)
	{
		delete dialogue_panel;
		delete choices_panel;
	}
}


void DialogueSystem::handle_event(Event* event)
{
	switch (event->get_event_id())
	{
		case EventID::_SYSTEM_DIALOGUESYSTEM_UPDATE_:
			update();
			break;
		default:
			break;
	}
}


}