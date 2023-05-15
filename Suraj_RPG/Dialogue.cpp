#include "pch.h"
#include "Dialogue.h"

#include "core/FileManager.h"
#include "core/Debug.h"

namespace bm98
{

Dialogue::Dialogue(std::string file_name)
{
	load_dialogue(file_name);
	link_statements();
}

Dialogue::~Dialogue()
{
}

bool Dialogue::can_continue()
{
	if (current_statement->target_statement)
		return true;
	for (auto& c : current_statement->choices)
		if (c.target_statement)
			return true;
	
	return false;
}

bool Dialogue::statement_ended()
{
	if (current_statement->current_text == current_statement->text.size() - 1)
		return true;

	return false;
}

void Dialogue::next(int choice)
{
	if (!can_continue())
	{
		core::Debug::Instance()->log("[Dialogue] Next failed due to cannot continue", core::Debug::LogLevel::WARNING);
		return;
	}
	if (current_statement->current_text < current_statement->text.size() - 1)
	{
		current_statement->current_text++;
		return;
	}
	statements_displayed.push(current_statement);
	current_statement->clean_up();
	
	if (current_statement->target_statement)
		current_statement = current_statement->target_statement;
	else
	{
		current_statement = current_statement->choices[choice].target_statement;
	}
}

std::string Dialogue::get_text()
{
	if (!current_statement)
	{
		core::Debug::Instance()->log("[Dialogue] Next failed due to no current statement", core::Debug::LogLevel::WARNING);
		return "";
	}
	return current_statement->text[current_statement->current_text];
}

std::vector<std::string> Dialogue::get_choices()
{
	std::vector<std::string> choices;
	if (current_statement)
	{
		for (auto& c : current_statement->choices)
		{
			choices.push_back(c.text);
		}
	}

	return choices;
}

void Dialogue::load_dialogue(std::string file_name)
{
	Json::Value dialogue;

	dialogue = core::FileManager::Instance()->load_from_file("Resources/Dialogue/"+file_name);

	for (Json::Value statement : dialogue["statements"])
	{
		std::string name = statement["name"].asString();
		std::string text = statement["text"].asString();

		Statement new_statement;

		new_statement.identifier = name;
		//new_statement.text = text;

		for (Json::Value line : statement["texts"])
			new_statement.text.push_back(line.asString());

		if (!statement["statement-link"].isNull())
			new_statement.target_statement_name = statement["statement-link"].asString();
		
		std::vector<Choice> choices(statement["choices"].size());
		for (Json::Value choice : statement["choices"])
		{
			int index = choice["index"].asInt64();

			std::string text = choice["text"].asString();
			//Statement* owner_link = &new_statement;

			Choice new_choice;
			new_choice.text = text;
			new_choice.target_statement_name = choice["statement-link"].asString();
			
			//std::vector<core::EventID> event_ids;
			//for (Json::Value choice_event : choice["events"])
			//{
			//	event_ids.push_back(core::ToEventID(choice_event.asString()));
			//}

			choices[index] = new_choice;
		}

		new_statement.choices = choices;

		statements[name] = new_statement;
		
		if (statement["entry"].asBool() == true)
			entry_statements.push_back(&statements[name]);

	}

}

void Dialogue::link_statements()
{
	for (auto& statement : statements)
	{
		if (statement.second.target_statement_name.size() > 0)
		{
			if (statements.find(statement.second.target_statement_name) != statements.end())
			{
				statement.second.target_statement = &statements[statement.second.target_statement_name];
			}
		}
		for (auto& choice : statement.second.choices)
		{
			if (statements.find(choice.target_statement_name) != statements.end())
			{
				choice.target_statement = &statements[choice.target_statement_name];
				continue;
			}
			if (statements.find(choice.target_statement_name) == statements.end() &&
				!choice.target_statement_name.empty())
			{
				core::Debug::Instance()->log("[Dialogue] Failed to find target statement", core::Debug::LogLevel::WARNING);
			}

		}
	}
}

void Dialogue::find_entry_statement()
{
	current_statement = entry_statements[0];
}

}