#pragma once
namespace bm98::core
{

//#define EVENTID std::string

enum class EventID
{

#pragma region System
	
	/// <summary>System event to process all events for the previous frame.</summary>
	_SYSTEM_EVENTSYSTEM_PROCESS_EVENTS_,
	_SYSTEM_RENDER_,
	_SYSTEM_UPDATE_TIME_,
	_SYSTEM_UPDATE_INPUT_,
	_SYSTEM_LATE_UPDATE_INPUT_,

	_SYSTEM_SETTINGS_WINDOW_SIZE_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_MASTER_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_MUSIC_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_SOUND_CHANGE_,


	_TIME_INITIALIZE_,
	_TIME_UPDATE_,
	_TIME_RESET_SINCE_STATE_CHANGE,



	_SYSTEM_SHUTDOWN = 999,

#pragma endregion

#pragma region Scene

	/// <summary>Scene has added a new object.</summary>
	SCENE_ADDED_GAMEOBJECT,	
	/// <summary>Scene has removed an object.</summary>
	SCENE_REMOVED_GAMEOBJECT,
	/// <summary>Scene has changed.</summary>
	SCENE_CHANGE,
	SCENE_ORDER_CHANGE,

#pragma endregion

#pragma region Gameobject

	/// <summary>GameObject has been instantiated through SceneManager.</summary>
	GAMEOBJECT_INSTANTIATE,
	/// <summary>GameObject has been destroyed through SceneManager.</summary>
	GAMEOBJECT_DESTROY,
	/// <summary>GameObject has changed its parent object.</summary>
	GAMEOBJECT_PARENT_CHANGE,
	/// <summary>GameObject has had a component added.</summary>
	GAMEOBJECT_COMPONENT_ADDED,
	/// <summary>GameObject has had a component removed.</summary>
	GAMEOBJECT_COMPONENT_REMOVED,

#pragma endregion


};


class Event
{
public:
	Event(EventID event_id, void* parameter = nullptr, void* caller = nullptr);
	virtual ~Event();

	inline EventID get_event_id() const;
	void* get_parameter();
	/// <summary>
	/// If the event provided a caller reference when called, this value will contain
	/// a reference to the object which pushed the event.
	/// </summary>
	/// <returns></returns>
	void* get_caller();

private:
	EventID event_id;
	void* parameter;
	void* caller;

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
	/// Adds an event of type event_id to be sent to all subscribed listeners 
	/// at the start of the next update.
	/// EXCEPTIONS: System events will be handled instantly when pushed.
	/// Events pushed from within a handle_event() call will be handled in the same
	/// frame.
	/// </summary>
	void push_event(EventID event_id, void* data = nullptr, void* caller = nullptr);

	


private:
	EventSystem() {}
	~EventSystem(){ shutdown(); }
	EventSystem(const EventSystem& rhs)
	{

	}
	EventSystem& operator=(const EventSystem& rhs) {}

	void handle_system_events(Event event);

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