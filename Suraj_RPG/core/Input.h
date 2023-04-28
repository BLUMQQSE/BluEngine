#pragma once
#include "EventSystem.h"
#include "../Timer.h"
namespace bm98::core
{
class Input : public Listener
{
public:

	enum class Mouse : unsigned char
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

public:
	// Returns instance of the Input
	static Input* Instance()
	{
		static Input instance;
		return &instance;
	}

	sf::RenderWindow* window;

	sf::Vector2f mouse_position(sf::View* view = NULL);
	sf::Vector2i mouse_position_screen();
	sf::Vector2i mouse_position_window();
	sf::Vector2i mouse_position_grid(float& grid_size, sf::View* view = NULL);

	/// <summary>
	/// Returns true if the specified key is pressed. The provided key
	/// corresponds to the current state keybinds, which can be found in 
	/// the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is held down this frame.</returns>
	bool get_action(std::string key);
	/// <summary>
	/// Returns true if the specified key is first pressed down this frame.
	/// The provided key corresponds to the current state keybinds,
	///  which can be found in the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is first pressed this frame.</returns>
	bool get_action_down(std::string key);
	/// <summary>
	/// Returns true if the specified key is first released this frame.
	/// The provided key corresponds to the current state keybinds,
	///  which can be found in the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is first released this frame.</returns>
	bool get_action_up(std::string key);
	/// <summary>
	/// Returns true if the specified mouse button is pressed. 
	/// The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being held.</param>
	/// <param name="duration">: Duration the key has been pressed thus
	/// far. If not provided, no duration is set.</param>
	/// <returns>True if mouse is held down this frame.</returns>
	bool get_mouse(Mouse mouse_button = Mouse::LEFT, float* duration = nullptr);
	/// <summary>
	/// Returns true if the specified mouse button is first pressed down 
	/// this frame. The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being pressed.</param>
	/// <returns>True if mouse is first pressed this frame.</returns>
	bool get_mouse_down(Mouse mouse_button = Mouse::LEFT);
	/// <summary>
	/// Returns true if the specified mouse button is first released 
	/// this frame. The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being released.</param>
	/// <param name="duration">: Duration the key has been pressed thus
	/// far. If not provided, no duration is set.</param>
	/// <returns>True if mouse button is released this frame.</returns>
	bool get_mouse_up(Mouse mouse_button = Mouse::LEFT, float* duration = nullptr);
	
	bool get_double_click();

	float get_mouse_scroll_delta();

	void load_keybinds_from_file(std::string keybind_state_name,
		std::string file_path);
	void load_supported_keybinds_from_file(std::string file_path);

	/// <summary>
	/// Changes current keybind list with which checking for keyboard
	/// input will be used. Expected to be called by Game.cpp when states count
	/// has changed.
	/// </summary>
	/// <param name="keybinds">Name of keybind set.</param>
	void change_keybinds_state(std::string keybinds);
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	const bool using_input_box();
	void set_using_input_box(bool val);
	const sf::RenderWindow* get_window() const;

protected:

private:
	enum class PressedState
	{
		UNPRESSED,
		PRESSED_FRAME,
		PRESSED,
		RELEASED_FRAME,
		RESET
	};

	Input();
	~Input() {}
	Input(const Input& rhs)
	{

	}
	Input& operator=(const Input& rhs) {}

	/// <summary>
	/// Current position of mouse on the screen.
	/// </summary>
	sf::Vector2i mouse_pos_screen;
	/// <summary>
	/// Current position of the mouse on the game window.
	/// </summary>
	sf::Vector2i mouse_pos_window;
	/// <summary>
	/// Current position of the mouse of the view.
	/// </summary>
	sf::Vector2f mouse_pos_view;

	std::unordered_map<std::string, int> supported_keys;
	std::unordered_map<std::string, int>* current_keybinds;
	std::map<std::string, PressedState> key_states;

	//contains all keybinds
	std::unordered_map<std::string, std::unordered_map<std::string, int>> keybinds;

	PressedState mouse_left;
	PressedState mouse_right;
	PressedState mouse_middle;

	//float mouse_left_delta;
	//float mouse_right_delta;
	//float mouse_middle_delta;

	Timer mouse_left_timer;
	Timer mouse_right_timer;
	Timer mouse_middle_timer;

	float mouse_scroll_delta;

	Timer time_since_left_clicked;

	bool in_input_box;

	/// <summary>
	/// Checks which mouse buttons are currently held down
	/// </summary>
	void update_mouse_input();
	/// <summary>
	/// Checks which keyboard buttons are currently held down
	/// </summary>
	void update_key_input();

	void update_mouse_states();
	void update_key_states();


	// Inherited via Listener
	virtual void handle_event(Event* event) override;


	void init(sf::RenderWindow* window_ptr);
	void update();
	void update_mouse_scroll(int scroll_delta);
	/// <summary>
	/// Progresses any Input states which were altered last frame.
	/// </summary>
	void reset_update();

};

}