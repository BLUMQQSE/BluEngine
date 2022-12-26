#pragma once
namespace bm98::core
{
class Input
{
public:

	enum class Mouse : unsigned char
	{
		LEFT,
		RIGHT,
		MIDDLE
	};

public:

	static sf::RenderWindow* window;
	Input();
	virtual ~Input();

	static void init(sf::RenderWindow* window_ptr);
	static void update();
	static void update_mouse_scroll(int scroll_delta);
	static void late_update();

	static sf::Vector2f mouse_position(sf::View* view = NULL);
	static sf::Vector2i mouse_position_screen();
	static sf::Vector2i mouse_position_window();
	static sf::Vector2i mouse_position_grid(float& grid_size, sf::View* view = NULL);

	/// <summary>
	/// Returns true if the specified key is pressed. The provided key
	/// corresponds to the current state keybinds, which can be found in 
	/// the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is held down this frame.</returns>
	static bool get_action(std::string key);
	/// <summary>
	/// Returns true if the specified key is first pressed down this frame.
	/// The provided key corresponds to the current state keybinds,
	///  which can be found in the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is first pressed this frame.</returns>
	static bool get_action_down(std::string key);
	/// <summary>
	/// Returns true if the specified key is first released this frame.
	/// The provided key corresponds to the current state keybinds,
	///  which can be found in the corresponding config file.
	/// </summary>
	/// <param name="key">: String relating to an sfml key pressed.</param>
	/// <returns>True if key is first released this frame.</returns>
	static bool get_action_up(std::string key);

	/// <summary>
	/// Returns true if the specified mouse button is pressed. 
	/// The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being held.</param>
	/// <param name="duration">: Duration the key has been pressed thus
	/// far. If not provided, no duration is set.</param>
	/// <returns>True if mouse is held down this frame.</returns>
	static bool get_mouse(Mouse mouse_button, float* duration = nullptr);
	/// <summary>
	/// Returns true if the specified mouse button is first pressed down 
	/// this frame. The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being pressed.</param>
	/// <returns>True if mouse is first pressed this frame.</returns>
	static bool get_mouse_down(Mouse mouse_button);
	/// <summary>
	/// Returns true if the specified mouse button is first released 
	/// this frame. The provided button corrisponds to sfml::Mouse button.
	/// </summary>
	/// <param name="mouse_button">: Mouse button being released.</param>
	/// <param name="duration">: Duration the key has been pressed thus
	/// far. If not provided, no duration is set.</param>
	/// <returns>True if mouse button is released this frame.</returns>
	static bool get_mouse_up(Mouse mouse_button, float* duration = nullptr);
	static float get_mouse_scroll_delta();

	static void load_keybinds_from_file(std::string keybind_state_name,
		std::string file_path);
	static void load_supported_keybinds_from_file(std::string file_path);

	/// <summary>
	/// Changes current keybind list with which checking for keyboard
	/// input will be used. Expected to be called by Game.cpp when states count
	/// has changed.
	/// </summary>
	/// <param name="keybinds">Name of keybind set.</param>
	static void change_keybinds_state(std::string keybinds);
	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	static const bool using_input_box();
	static void set_using_input_box(bool val);

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

	/// <summary>
	/// Current position of mouse on the screen.
	/// </summary>
	static sf::Vector2i mouse_pos_screen;
	/// <summary>
	/// Current position of the mouse on the game window.
	/// </summary>
	static sf::Vector2i mouse_pos_window;
	/// <summary>
	/// Current position of the mouse of the view.
	/// </summary>
	static sf::Vector2f mouse_pos_view;

	static std::unordered_map<std::string, int> supported_keys;
	static std::unordered_map<std::string, int>* current_keybinds;
	static std::map<std::string, PressedState> key_states;

	//contains all keybinds
	static std::unordered_map<std::string, std::unordered_map<std::string, int>> keybinds;

	static PressedState mouse_left;
	static PressedState mouse_right;
	static PressedState mouse_middle;

	static float mouse_left_delta;
	static float mouse_right_delta;
	static float mouse_middle_delta;
	static float mouse_scroll_delta;

	static bool in_input_box;

	/// <summary>
	/// Checks which mouse buttons are currently held down
	/// </summary>
	static void update_mouse_input();
	/// <summary>
	/// Checks which keyboard buttons are currently held down
	/// </summary>
	static void update_key_input();

	static void update_mouse_states();
	static void update_key_states();

	static void update_mouse_deltas();

};

}