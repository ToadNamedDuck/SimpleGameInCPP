//Code written by ToadNamedDuck, using a tutorial from Dan Zaidan as reference as I follow his tutorial on Youtube.

//This file is the Windows platform-specific code layer - this is PLATFORM DEPENDENT.

//Since we are devloping using the Windows API, we can find resources for future API requests on MSDN.
//Since we are making a graphical interface for the Windows platform, we need to include the windows.h header, and declare a WinMain as our entry point.
#include <windows.h>
#include "utils.cpp"


bool running = true;

//We want our buffer variables global. void* is essentially "I don't care what you type it as" or similar to how I use var in another language.
//Rework into struct.
struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmapinfo;
};
Render_State render_state;
#include "renderer.cpp"
//It is included here because it relies on this global.

//Create our window callback function.
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) {//Interpret messages from the window for our program.
		//Close the window if we get the close or destroyed message
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		//This is the message for window changing size so we can get the proper buffer.
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;
			//Our buffer contains width*height pixels.
			//each pixel stores 32 bits of data. (unsigned int)

			int render_state_size = render_state.width * render_state.height * sizeof(u32);

			//We need to get a heap of memory from the operating system, and since this is windows, we can use the windows library for this.
			//Before we set our buffer memory, we need to check and see if the buffer has already been set, so we can release it and dynamically recreate our buffer size.
			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);

			//The first param has something to do with the address in memory we want to allocate, which we just set to 0 and I think get a fresh page of memory.
			//Second param is our calculated buffer size
			//Third is our ability to reserve a chunk of memory, and commit/reserve the section at one time, which means memory in this section will be initialized to 0.
			//Fourth is the protection level, and PAGE_READWRITE allows us to read and write to a dynamically sized section like this one :D.
			render_state.memory = VirtualAlloc(0, render_state_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			//Now that memory is allocated, we need to generate our bitmapinfo pieces, and most of the data is in its header structure.
			render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
			render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
			render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
			render_state.bitmapinfo.bmiHeader.biPlanes = 1;
			render_state.bitmapinfo.bmiHeader.biBitCount = 32; //32 since we are running on the size of a 32 bit integer.
			render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;

			//pass the pointer of this to our function in the render step of the game loop.
		} break;
			//If we don't, we just return default result.
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
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
	//We store the created window to a variable for later use.
	HWND window = CreateWindow(window_class.lpszClassName, L"My First Game in C++", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	//To create our window context, we need to use a function that returns it in context to our created window.
	HDC device_context = GetDC(window);


	while (running) {
		//This is where we create our game loop!

		//Get input
		//During this step, we want to process messages we got and haven't dealt with yet.
		MSG message;
		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {//PeekMessage gives us all of the messages we haven't read yet, and we set the message to the ref, tell it from our window we made, tell it filters, which are 0, and what to do with the messages, which is get rid of them.
			TranslateMessage(&message);//Translate the message
			DispatchMessage(&message);//Dispatch the message to our callback!
		}

		//Simulate
		render_background();
		clear_screen(0xc285d3);
		draw_rect(50, 50, 200, 500, 0x00ff00);
		//Render
		// 
		//To render, we need to get the buffer, fill it, and send it to the OS to handle.
		//To get the proper buffer, we need to get the proper size of the window, which the player could change, so to properly handle that, get it every cycle from messages sent to window.
		//That happens in the proc on a wm size message case.
		
		//Needs a handle to a device context, which is how windows recognizes our window.
		//X/Y destinations, which we set to 0 (which should be top left of our window.)
		//Width/Height of our window.
		//IDK what xSrc and ySrc are but we set them to 0
		//Width and Height src should be the same as the buffer size, since our buffer is the same size as our window.
		//The memory that we are passing from goes into the next spot
		//The bitmap info we created in our proc goes in
		//The usage, which is rgb colors
		//Finally, the operation to perform, which is copy from the buffer into the window.
		StretchDIBits(device_context, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

	}
}