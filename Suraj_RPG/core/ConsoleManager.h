#pragma once
namespace bm98::core
{

class ConsoleManager
{
public:
	// Returns instance of the Input
	static ConsoleManager* Instance()
	{
		static ConsoleManager instance;
		return &instance;
	}


private:
	ConsoleManager();
	~ConsoleManager() {}
	ConsoleManager(const ConsoleManager& rhs)
	{

	}
	ConsoleManager& operator=(const ConsoleManager& rhs) {}


};

}