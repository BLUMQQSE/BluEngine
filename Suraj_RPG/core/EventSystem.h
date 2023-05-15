#pragma once
#include "../globals.h"
namespace bm98
{


}
namespace bm98::core
{

//#define EVENTID std::string

struct Caller
{

#pragma region ENUM
	enum class Name
	{
		GAMECLOCK,
		GAMEOBJECT,
		BOX_COLLIDER_COMPONENT,
		CAPSULE_COLLIDER_COMPONENT,
		COLLIDER_COMPONENT,
		RIGIDBODY_COMPONENT,
		TILEMAP_COLLIDER_COMPONENT,
		TILEMAP_COMPONENT,
		SPRITE_COMPONENT,
		ANIMATED_SPRITE_COMPONENT,
		ANIMATION_COMPONENT,
		CHILD_ANIMATION_COMPONENT,
		CAMERA_COMPONENT,
		AUDIO_SOURCE,
		DONT_DESTROY_COMPONENT,
		DESTRUCTOR,
		CHEST,
		INTERACTOR,
		SCENE_CHANGE,
		ITEM_CONTROLLER,
		WEAPON_CONTROLLER,
		INVENTORY,
		COMBAT_INVENTORY,
		INVENTORY_GUI_CONTROLLER,
		INVENTORY_WINDOW,
		PLAYER_CONTROLLER,
		_LAST_DONT_REMOVE
	};

	static std::string ToString(Name name)
	{
		switch (name)
		{
		case Name::GAMEOBJECT:
			return "GAMEOBJECT";
		case Name::BOX_COLLIDER_COMPONENT:
			return "BOX_COLLIDER_COMPONENT";
		case Name::CAPSULE_COLLIDER_COMPONENT:
			return "CAPSULE_COLLIDER_COMPONENT";
		case Name::COLLIDER_COMPONENT:
			return "COLLIDER_COMPONENT";
		case Name::RIGIDBODY_COMPONENT:
			return "RIGIDBODY_COMPONENT";
		case Name::TILEMAP_COLLIDER_COMPONENT:
			return "TILEMAP_COLLIDER_COMPONENT";
		case Name::TILEMAP_COMPONENT:
			return "TILEMAP_COMPONENT";
		case Name::SPRITE_COMPONENT:
			return "SPRITE_COMPONENT";
		case Name::ANIMATED_SPRITE_COMPONENT:
			return "ANIMATED_SPRITE_COMPONENT";
		case Name::ANIMATION_COMPONENT:
			return "ANIMATION_COMPONENT";
		case Name::CHILD_ANIMATION_COMPONENT:
			return "CHILD_ANIMATION_COMPONENT";
		case Name::CAMERA_COMPONENT:
			return "CAMERA_COMPONENT";
		case Name::AUDIO_SOURCE:
			return "AUDIO_SOURCE";
		case Name::DONT_DESTROY_COMPONENT:
			return "DONT_DESTROY_COMPONENT";
		case Name::DESTRUCTOR:
			return "DESTRUCTOR";
		case Name::CHEST:
			return "CHEST";
		case Name::INTERACTOR:
			return "INTERACTOR";
		case Name::SCENE_CHANGE:
			return "SCENE_CHANGE";
		case Name::ITEM_CONTROLLER:
			return "ITEM_CONTROLLER";
		case Name::WEAPON_CONTROLLER:
			return "WEAPON_CONTROLLER";
		case Name::INVENTORY:
			return "INVENTORY";
		case Name::COMBAT_INVENTORY:
			return "COMBAT_INVENTORY";
		case Name::INVENTORY_GUI_CONTROLLER:
			return "INVENTORY_GUI_CONTROLLER";
		case Name::INVENTORY_WINDOW:
			return "INVENTORY_WINDOW";
		case Name::PLAYER_CONTROLLER:
			return "PLAYER_CONTROLLER";
		default:
			std::cout << "ERROR::GLOBAL::TOSTRING::Component NAME not registered\n";
			return "";
		}
	}

	static Name ToName(std::string name)
	{
		if (name == "GAMEOBJECT")
			return Name::GAMEOBJECT;
		if (name == "BOX_COLLIDER_COMPONENT")
			return Name::BOX_COLLIDER_COMPONENT;
		if (name == "CAPSULE_COLLIDER_COMPONENT")
			return Name::CAPSULE_COLLIDER_COMPONENT;
		if (name == "COLLIDER_COMPONENT")
			return Name::COLLIDER_COMPONENT;
		if (name == "RIGIDBODY_COMPONENT")
			return Name::RIGIDBODY_COMPONENT;
		if (name == "TILEMAP_COLLIDER_COMPONENT")
			return Name::TILEMAP_COLLIDER_COMPONENT;
		if (name == "TILEMAP_COMPONENT")
			return Name::TILEMAP_COMPONENT;
		if (name == "SPRITE_COMPONENT")
			return Name::SPRITE_COMPONENT;
		if (name == "ANIMATED_SPRITE_COMPONENT")
			return Name::ANIMATED_SPRITE_COMPONENT;
		if (name == "ANIMATION_COMPONENT")
			return Name::ANIMATION_COMPONENT;
		if (name == "CHILD_ANIMATION_COMPONENT")
			return Name::CHILD_ANIMATION_COMPONENT;
		if (name == "CAMERA_COMPONENT")
			return Name::CAMERA_COMPONENT;
		if (name == "AUDIO_SOURCE")
			return Name::AUDIO_SOURCE;
		if (name == "DONT_DESTROY_COMPONENT")
			return Name::DONT_DESTROY_COMPONENT;
		if (name == "DESTRUCTOR")
			return Name::DESTRUCTOR;
		if (name == "CHEST")
			return Name::CHEST;
		if (name == "INTERACTOR")
			return Name::INTERACTOR;
		if (name == "SCENE_CHANGE")
			return Name::SCENE_CHANGE;
		if (name == "ITEM_CONTROLLER")
			return Name::ITEM_CONTROLLER;
		if (name == "WEAPON_CONTROLLER")
			return Name::WEAPON_CONTROLLER;
		if (name == "INVENTORY")
			return Name::INVENTORY;
		if (name == "COMBAT_INVENTORY")
			return Name::COMBAT_INVENTORY;
		if (name == "INVENTORY_GUI_CONTROLLER")
			return Name::INVENTORY_GUI_CONTROLLER;
		if (name == "INVENTORY_WINDOW")
			return Name::INVENTORY_WINDOW;
		if (name == "PLAYER_CONTROLLER")
			return Name::PLAYER_CONTROLLER;

		std::cout << "ERROR::GLOBAL::TOSTRING::Component NAME not registered\n";
		return Name::_LAST_DONT_REMOVE;

	}

#pragma endregion

	Caller()
	{

	}
	Caller(Name name, void* pointer)
	{
		this->name = name;
		this->pointer = pointer;
	}
	Name name;
	void* pointer = nullptr;
};

enum class EventID
{
#pragma region System
	
	/// <summary>System event to process all events for the previous frame.</summary>
	_SYSTEM_EVENTSYSTEM_PROCESS_EVENTS_,

#pragma region Time

	_SYSTEM_TIME_INITIALIZE_,
	_SYSTEM_TIME_UPDATE_,
	_SYSTEM_TIME_FIXED_UPDATE_,
	_SYSTEM_TIME_RESET_FIXED_,
	_SYSTEM_TIME_APPLY_SCALE_,
	_SYSTEM_TIME_RESET_SINCE_STATE_CHANGE_,

#pragma endregion

#pragma region Input

	_SYSTEM_INPUT_INITIALIZE_,
	_SYSTEM_INPUT_UPDATE_,
	_SYSTEM_INPUT_RESET_UPDATE_,
	_SYSTEM_INPUT_UPDATE_SCROLL_,

#pragma endregion

#pragma region Renderer

	_SYSTEM_RENDERER_INITIALIZE_,
	_SYSTEM_RENDERER_REFRESH_,
	_SYSTEM_RENDERER_RENDER_,
	_SYSTEM_RENDERER_CLEAR_,
	_SYSTEM_RENDERER_CLEAR_GIZMOS_,
	_SYSTEM_RENDERER_UPDATE_TOP_UI_,
	_SYSTEM_RENDERER_FIXED_UPDATE_,
#pragma endregion

#pragma region ResourceManager

	_SYSTEM_RESOURCEMANAGER_LOAD_RESOURCES_,

#pragma endregion

#pragma region Physics

	_SYSTEM_PHYSICS_INITIALIZE_,
	_SYSTEM_PHYSICS_FIXED_UPDATE_,

#pragma endregion

#pragma region SceneManager

	_SYSTEM_SCENEMANAGER_INITIALIZE_,
	_SYSTEM_SCENEMANAGER_DESTROY_,
	_SYSTEM_SCENEMANAGER_CLEAR_ACTIVE_SCENE_,
	_SYSTEM_SCENEMANAGER_PRE_SCENE_CHANGE_FLAG_,
	_SYSTEM_SCENEMANAGER_POST_SCENE_CHANGE_FLAG_,

#pragma endregion

#pragma region UITagSystem

	_SYSTEM_UITAG_UPDATE_,

#pragma endregion

#pragma region DialogueSystem

	_SYSTEM_DIALOGUESYSTEM_UPDATE_,

#pragma endregion

#pragma region STATE

	_SYSTEM_GAME_PRE_STATE_CHANGE_FLAG_, // State* parameter
	_SYSTEM_GAME_POST_STATE_CHANGE_FLAG_, // State* parameter

#pragma endregion


	_SYSTEM_SETTINGS_WINDOW_SIZE_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_MASTER_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_MUSIC_CHANGE_,
	_SYSTEM_SETTINGS_AUDIO_SOUND_CHANGE_,


	_SYSTEM_SHUTDOWN = 999,

#pragma endregion

#pragma region Scripts

	SCRIPTS_LOAD_SCENE,
	SCRIPTS_SAVE_SCENE,

#pragma endregion

#pragma region Interaction

	INTERACTION_INVENTORY_UPDATED,
	INTERACTION_EXIT_INTERACTION,

#pragma endregion

#pragma region Scene

	SCENE_ADDED_GAMEOBJECT_FLAG, // Scene has added a new object
	SCENE_REMOVED_GAMEOBJECT_FLAG, // Scene has removed an object
	SCENE_CHANGE_FLAG, // Scene has changed
	SCENE_GAMEOBJECT_ORDER_CHANGE_FLAG, // Order of objects in scene have changed
		
#pragma endregion

#pragma region Gameobject

	GAMEOBJECT_INSTANTIATE,
	GAMEOBJECT_DESTROY,
	GAMEOBJECT_PARENT_CHANGE,

	GAMEOBJECT_COMPONENT_ADDED_FLAG,
	GAMEOBJECT_COMPONENT_REMOVED_FLAG,


#pragma endregion

#pragma region Animation
	/// <summary>
	///  Returns animation name as a parameter, or nullptr if AnimatedSprite, and the Component as caller.
	///</summary>
	ANIMATION_FRAME_CHANGE,
	/// <summary>
	/// Returns animation name as a parameter, or nullptr if AnimatedSprite, and a Component as caller.
	/// </summary>
	ANIMATION_COMPLETE,

#pragma endregion

#pragma region GameClock

	GAMECLOCK_MIN_CHANGE,
	GAMECLOCK_HOUR_CHANGE,
	GAMECLOCK_DAY_CHANGE,
	GAMECLOCK_DAYPART_CHANGE,

#pragma endregion


};

class Event
{
public:

	Event(EventID event_id, void* parameter = nullptr, Caller caller = Caller());
	virtual ~Event() {}

	inline EventID get_event_id() const { return event_id; }
	void* get_parameter() { return parameter; }
	/// <summary>
	/// If the event provided a caller reference when called, this value will contain
	/// a reference to the object which pushed the event.
	/// </summary>
	/// <returns></returns>
	//void* get_caller() { return caller; }
	Caller get_caller() { return caller; }


private:
	
	EventID event_id;
	void* parameter;
	//void* caller;
	Caller caller;
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
	static EventSystem* Instance()
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
	void push_event(EventID event_id, void* data = nullptr, Caller caller = Caller());


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