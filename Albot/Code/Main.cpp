
#include <iostream>
#include "Managers/WindowManager.h"
//#include "Managers/EventManager.h"

int main()
{
	std::cout << "Main is running" << std::endl;
	WindowManager Window;

#ifdef DEVELOPER
	std::cout << "DEVELOPER MODE RUNNNING" << std::endl;
#endif // DEVELOPER


	Window.Run();
	return 0;
}
