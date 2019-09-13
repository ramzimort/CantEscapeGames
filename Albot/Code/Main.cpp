#include <iostream>
#include "Managers/WindowManager.h"

int main()
{
	std::cout << "Main is running" << std::endl;
	WindowManager Window;

	Window.Run();
	return 0;
}
