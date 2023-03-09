#pragma once
namespace bm98::core
{

//#define EVENTID std::string

enum class EventID
{
#pragma region System
	
	_EVENTSYSTEM_PROCESS_EVENTS_,

	_TIME_INITIALIZE_,
	_TIME_UPDATE_,
	_TIME_RESET_SINCE_STATE_CHANGE,

#pragma endregion


	SCENE_ADD_GAMEOBJECT,
	SCENE_REMOVE_GAMEOBJECT,
	SCENE_CHANGE,
};


class Event
{
public:
	Event(EventID event_id, void* parameter = 0);
	virtual ~Event();

	inline EventID get_event_id() const;
	inline void* get_parameter();

private:
	EventID event_id;
	void* parameter;

};

class Listener
{
public:
	Listener();
	virtual ~Listener();
	/// <summary>
	/// Recieves events from the EventSystem after they've been push in the previous frame.
	/// </summary>
	/// <param name="event"></param>
	virtual void handle_event(Event* event) = 0;

private:

};

class EventSystem
{
public:
	// Returns instance of the EventSystem
	static EventSystem* instance()
	{
		static EventSystem instance;
		return &instance;
	}

	/// <summary>
	/// Registers a Listener to a provided EventID. Will listen for events of type event_id.
	/// </summary>
	void subscribe(EventID event_id, Listener* client);

	/// <summary>
	/// Unregisters a Listener from a provided EventID. Will stop listening
	/// for events of type event_id.
	/// </summary>
	void unsubscribe(EventID event_id, Listener* client);
	
	/// <summary>
	/// Unregisters a Listener from listening for any events.
	/// </summary>
	/// <param name="client"></param>
	void unsubscribe_all(Listener* client);

	/// <summary>
	/// Adds an event of type event_id to be sent to all subscribed listeners at the start of the
	/// next update.
	/// </summary>
	void push_event(EventID event_id, void* data = 0);

	


private:
	EventSystem() {}
	~EventSystem(){ shutdown(); }
	EventSystem(const EventSystem& rhs)
	{

	}
	EventSystem& operator=(const EventSystem& rhs) {}

	bool check_system_events(Event event);

	void process_events();
	void clear_events();

	void shutdown();

	bool already_registered(EventID event_id, Listener* client);
	void dispatch_event(Event* event);

	// Database of clients and their events
	std::multimap<EventID, Listener*> database;
	// List of events to be processed
	std::list<Event> current_events;

};

}