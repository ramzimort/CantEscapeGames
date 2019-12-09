#pragma once

class InputQueue
{
public:
	InputQueue();
	~InputQueue();

public:
	std::queue<SDL_Event> Events;
};