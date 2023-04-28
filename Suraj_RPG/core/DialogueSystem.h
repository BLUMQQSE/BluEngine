#pragma once

namespace bm98::core
{

class DialogueSystem
{
public:
	// Returns instance of the DialogueSystem
	static DialogueSystem* Instance()
	{
		static DialogueSystem instance;
		return &instance;
	}

private:
	DialogueSystem() {}
	~DialogueSystem() { shutdown(); }
	DialogueSystem(const DialogueSystem& rhs)
	{

	}
	DialogueSystem& operator=(const DialogueSystem& rhs) {}

	void shutdown();

};

}
