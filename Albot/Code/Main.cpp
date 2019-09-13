
#include <iostream>
#include "Managers/WindowManager.h"
//#include "Managers/EventManager.h"

int main()
{
	std::cout << "Main is running" << std::endl;
	WindowManager Window;
	//EventManager M2;

	Window.Run();
	return 0;
}
