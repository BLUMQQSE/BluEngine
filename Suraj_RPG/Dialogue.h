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
	/// The dialogue continues beyond this statment.
	/// </summary>
	bool can_continue();
	/// <summary>
	/// All texts within this statement have been rendered.
	/// </summary>
	bool statement_ended();
	/// <summary>
	/// Progress to next statement in the dialogue.
	/// </summary>
	/// <param name="choice">Choice chosen for next dialogue. Defaults
	/// to 0 if no choice is provided.</param>
	void next(int choice = 0);

	std::string get_text();


	struct Choice;

	std::vector<std::string> get_choices();
	void find_entry_statement();

private:
	struct Statement;
	struct Choice
	{
		Choice() {}

		std::string get_text();

		std::string text;
		std::string target_statement_name;
		Statement* target_statement = nullptr;
		std::vector<core::EventID> events;

	};

	struct Statement
	{
		Statement()
		{

		}

		void clean_up()
		{
			current_text = 0; 
		}

		std::string identifier;
		int current_text = 0;
		std::vector<std::string> text;
		std::vector<Choice>choices;
		Statement* target_statement = nullptr;
		std::string target_statement_name;

	};

	std::unordered_map<std::string, Statement> statements;
	std::vector<Statement*> entry_statements;
	std::stack<Statement*> statements_displayed;

	Statement* current_statement;

	void load_dialogue(std::string file_name);
	void link_statements();


};

}