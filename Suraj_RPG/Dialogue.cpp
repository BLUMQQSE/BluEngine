#include "pch.h"
#include "Dialogue.h"
#include "FileManager.h"

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

void Dialogue::next(int choice)
{
	if (!current_statement)
		return;

	current_statement = current_statement->choices[choice].target_statement;
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

		std::vector<Choice> choices(statement["choices"].size());
		for (Json::Value choice : statement["choices"])
		{
			int index = choice["index"].asInt64();

			std::string text = choice["text"].asString();
			Statement* owner_link = &new_statement;

			Choice new_choice;
			new_choice.text = text;
			new_choice.owner_statement = owner_link;
			new_choice.target_statement_name = choice["statement-link"].asString();
			
			std::vector<core::EventID> event_ids;
			for (Json::Value choice_event : choice["events"])
			{
				event_ids.push_back(core::ToEventID(choice_event.asString()));
			}

			choices[index] = new_choice;
		}

		new_statement.choices = choices;

		statements[name] = new_statement;
	}

}

void Dialogue::link_statements()
{
	for (auto& statement : statements)
	{
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
				std::cout << "ERROR::DIALOGUE::LINK_STATEMENTS::TARGET_STATEMENT_NOT_FOUND\n";
			}

		}
	}
}

}