#pragma once

/**
 * @file InputManager.h
 * @author Ramzi Mourtada
 * @date 12/4/2019
 * @brief Runs on a separate thread. 
 * Responsible for injecting input events into the game event queue
 */

class GameWindowSizeEvent;

struct ControllerData
{
	ControllerData() : 
		m_pGameController(nullptr), m_currentState({ false }) { }
	ControllerData(SDL_GameController* pCtrlr) :
		m_pGameController(pCtrlr) , m_currentState({ false })
	{
	}
	SDL_GameController* m_pGameController;
	std::array<bool, SDL_CONTROLLER_BUTTON_MAX> m_currentState;
};

class InputManager
{
public:
	InputManager(const std::string& gameName, bool Fullscreen, int w, int h);
	~InputManager();
	/**
	 * @brief Thread wait until any input event. On event, the event info is dispatched to be caught at the end of this frame.
	 * 
	 */
	void Update();
	
	/**
	 * @brief Get the Pointer's location in screen coordinates
	 * 
	 * @return const int* (const Array Pointer of size 2)
	 */
	const int* GetPointerLocation() const;	
	/**
	 * @brief Get mouse position in screen space as vec2
	 * 
	 * @return Vector2 
	 */
	Vector2 GetPointerLocVec2() const;
	/**
	 * @brief Get mouse position change b/w frames as vec2
	 * 
	 * @return Vector2 
	 */
	Vector2 GetPointerDeltaVec2() const;

	bool IsKeyPressed(unsigned int KeyScanCode) const; 

	/** Check mouse button press */
	bool IsMousePressed(unsigned int MouseScanCode) const;
	/** Check mouse button trigger (rising edge) */
	bool IsMouseTriggered(unsigned int MouseScanCode) const;
	/** Check mouse button release (falling edge) */
	bool IsMouseReleased(unsigned int MouseScanCode) const;
	/** Returns scrolling amount in y-direction*/
	Sint32 GetMouseScroll() const;
	
	void ToggleFullscreenMode();
	void SetWindowSize(int width, int height);

	SDL_Window* GetWindow();
	/** Check if exit button is pressed (close window) */
	bool IsQuit() const;
	void Quit();
	void OnWindowResizeRequest(const GameWindowSizeEvent* e);

private:
	void UpdateMouseClickState();
private:
	SDL_Window* m_pWindow;
	// Keyboard
	std::array<bool, 512> m_keyboardState;
	// Mouse
	Uint8 m_mouseStateCurrent;
	Uint8 m_mouseStatePrevious;
	int m_mousePositionCurrent[2];
	int m_mousePositionPrevious[2];
	Sint32 m_mouseWheelY;
	
	// Joystick
	std::unordered_map<int32_t, ControllerData> m_controllerData;

	// Main Event
	SDL_Event m_event;
	bool m_quit;
};


