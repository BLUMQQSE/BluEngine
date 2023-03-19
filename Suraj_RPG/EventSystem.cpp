#include "pch.h"
#include "EventSystem.h"

namespace bm98::core
{

#pragma region Event

Event::Event(EventID event_id, void* parameter, void* caller)
{
	this->event_id = event_id;
	this->parameter = parameter;
	this->caller = caller;
}

Event::~Event()
{
}

EventID Event::get_event_id() const
{
	return event_id;
}

void* Event::get_parameter()
{
	return parameter;
}

void* Event::get_caller()
{
	return caller;
}


#pragma endregion

#pragma region Listener

Listener::Listener()
{

}

Listener::~Listener()
{
	EventSystem::Instance()->unsubscribe_all(this);
}


#pragma endregion

#pragma region EventSystem

void EventSystem::subscribe(EventID event_id, Listener* client)
{
	if (!client || already_registered(event_id, client))
		return;

	// Insert client into the database
	database.insert(std::make_pair(event_id, client));

}

void EventSystem::unsubscribe(EventID event_id, Listener* client)
{
	std::pair<std::multimap<EventID, Listener*>::iterator,
		std::multimap<EventID, Listener*>::iterator> range;

	range = database.equal_range(event_id);

	for (std::multimap<EventID, Listener*>::iterator iter = range.first;
		iter != range.second; iter++)
	{
		if ((*iter).second == client)
		{
			// Erase the client and exit
			iter = database.erase(iter);
			break;
		}
	}

}

void EventSystem::unsubscribe_all(Listener* client)
{
	std::multimap<EventID, Listener*>::iterator iter = database.begin();

	while (iter != database.end())
	{
		if ((*iter).second == client)
		{
			iter = database.erase(iter);
		}
		else
		{
			iter++;
		}
	}
}

void EventSystem::push_event(EventID event_id, void* data, void* caller)
{
	
	Event new_event(event_id, data, caller);

	if (static_cast<int>(event_id) < 1000)
		return handle_system_events(new_event);

	current_events.push_back(new_event);

}

void EventSystem::handle_system_events(Event event)
{
	switch (event.get_event_id())
	{
	case EventID::_SYSTEM_EVENTSYSTEM_PROCESS_EVENTS_:
		process_events();
		break;
	default:
		if (event.get_event_id() < EventID::_SYSTEM_SETTINGS_WINDOW_SIZE_CHANGE_)
		{
			dispatch_event(&event);
		}
		break;
	}

}

void EventSystem::process_events()
{
	while (current_events.size() > 0)
	{
		dispatch_event(&current_events.front());
		current_events.pop_front();
	}
}

void EventSystem::clear_events()
{
	current_events.clear();
}

void EventSystem::shutdown()
{
	database.clear();
	current_events.clear();
}

bool EventSystem::already_registered(EventID event_id, Listener* client)
{
	bool registered = false;

	std::pair<std::multimap<EventID, Listener*>::iterator,
		std::multimap<EventID, Listener*>::iterator> range;

	range = database.equal_range(event_id);

	for (std::multimap<EventID, Listener*>::iterator iter = range.first;
		iter != range.second; iter++)
	{
		if ((*iter).second == client)
		{
			registered = true;
			break;
		}
	}
	return registered;
}

void EventSystem::dispatch_event(Event* event)
{
	std::pair< std::multimap<EventID, Listener*>::iterator,
		std::multimap<EventID, Listener*>::iterator> range;

	// Get all clients of the event
	range = database.equal_range(event->get_event_id());

	// Dispatch event to all listeners
	for (std::multimap<EventID, Listener*>::iterator iter = range.first; iter != range.second; iter++)
	{
		(*iter).second->handle_event(event);
	}

}


#pragma endregion






}