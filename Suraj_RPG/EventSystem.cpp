#include "pch.h"
#include "EventSystem.h"
#include "GameObject.h"

namespace bm98::core
{

#pragma region Event

Event::Event(EventID event_id, void* parameter)
{
	this->event_id = event_id;
	this->parameter = parameter;
}

Event::~Event()
{
}

EventID Event::get_event_id() const
{
	return event_id;
}

inline void* Event::get_parameter()
{
	return parameter;
}


#pragma endregion

#pragma region Listener

Listener::Listener()
{

}

Listener::~Listener()
{
	EventSystem::instance()->unsubscribe_all(this);
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

void EventSystem::push_event(EventID event_id, void* data)
{
	

	Event new_event(event_id, data);

	if (check_system_events(new_event))
		return;

	current_events.push_back(new_event);

}

bool EventSystem::check_system_events(Event event)
{
	if (event.get_event_id() == EventID::_EVENTSYSTEM_PROCESS_EVENTS_)
	{
		process_events();
		return true;
	}
	return false;
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