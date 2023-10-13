//Code written by ToadNamedDuck, using a tutorial from Dan Zaidan as reference as I follow his tutorial on Youtube.

//This file is the Windows platform-specific code layer - this is PLATFORM DEPENDENT.

//Since we are devloping using the Windows API, we can find resources for future API requests on MSDN.
//Since we are making a graphical interface for the Windows platform, we need to include the windows.h header, and declare a WinMain as our entry point.
#include <windows.h>

//Create our window callback function.
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	//We need to create a window class
	WNDCLASS window_class = {};//Declare window
	window_class.style = CS_HREDRAW | CS_VREDRAW; //Allows us to draw to the window horizontally and vertically.
	window_class.lpszClassName = L"Game Window Class";//Give the window a name. L converts the type of the string.
	window_class.lpfnWndProc = window_callback;//This is the callback function that we use to receive communication from the window. every time something happens, like the windows receives input, this gets called.
	//Register the window class
		//What we are doing is passing the compiler the address, like a locker number, of our window class so that it may find or modify that information later.
	RegisterClass(&window_class);//We are registering a reference to window_class. RegisterClass uses the reference to get to our original window_class and register its information.
	//Create the window!

	//CreateWindow takes the class name of the window, the Title, style options, so overlapped and visible, we let windows decide where to spawn the window, width height
	//We give it 0 for both WndParent (theres no parent) and for hMenu, pass our WinMain hInstance, and theres no lpParam for us.
	//That's a LOT
	CreateWindow(window_class.lpszClassName, L"My First Game in C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
}