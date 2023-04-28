#pragma once
#include "globals.h"
#include "core/EventSystem.h"
namespace bm98
{

class Dialogue
{
public:
	Dialogue(std::string file_name);
	virtual ~Dialogue();

	/// <summary>
	/// Progress to next statement in the dialogue.
	/// </summary>
	/// <param name="choice">Choice chosen for next dialogue. Defaults
	/// to 0 if no choice is provided.</param>
	void next(int choice = 0);

private:

	struct Statement;
	struct Choice
	{
		Choice()
		{
			
		}

		std::string text;
		Statement* owner_statement;
		std::string target_statement_name;
		Statement* target_statement = nullptr;
		std::vector<core::EventID> events;

	};

	struct Statement
	{
		Statement()
		{

		}

		std::string identifier;
		int current_text = 0;
		std::vector<std::string> text;
		std::vector<Choice>choices;

	};

	std::unordered_map<std::string, Statement> statements;
	Statement* current_statement;

	void load_dialogue(std::string file_name);
	void link_statements();


};

}