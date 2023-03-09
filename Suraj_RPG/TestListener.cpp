#include "pch.h"
#include "TestListener.h"
namespace bm98
{
TestListener::TestListener()
{
	EventSystem::instance()->register_client(EVENTID::Scene_add_gameobject, this);
}
TestListener::~TestListener()
{
}
void bm98::TestListener::handle_event(Event* event)
{
	std::cout << "evemt handled\n";
}

}