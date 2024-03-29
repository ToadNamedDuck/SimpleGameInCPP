//Code written by ToadNamedDuck, using a tutorial from Dan Zaidan as reference as I follow his tutorial on Youtube.
//This file is the Windows platform-specific code layer - this is PLATFORM DEPENDENT.
#include <windows.h>
#include "resource1.h"
#include <chrono>
#include <thread>
#include "utils.cpp"


 global_variable bool running = true;
 global_variable float MAXFPS = 90.f;
struct Render_State {
	int height, width;
	void* memory;

	BITMAPINFO bitmapinfo;
};
global_variable Render_State render_state;
#include "platform_common.cpp"
#include "renderer.cpp"
#include "game.cpp"

//Create our window callback function.
LRESULT CALLBACK window_callback(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch (uMsg) {
		case WM_CLOSE:
		case WM_DESTROY: {
			running = false;
		} break;
		case WM_SIZE: {
			RECT rect;
			GetClientRect(hwnd, &rect);
			render_state.width = rect.right - rect.left;
			render_state.height = rect.bottom - rect.top;

			int render_state_size = render_state.width * render_state.height * sizeof(u32);

			//We need to get a heap of memory from the operating system.
			//Before we set our buffer memory, we need to check and see if the buffer has already been set, so we can release it and dynamically recreate our buffer size.

			//I could add an option to set Fullscreen or windowed.
			if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);

			//The first param has something to do with the address in memory we want to allocate, which we just set to 0 and I think get a fresh page of memory.
			//Second param is our calculated buffer size
			//Third is our ability to reserve a chunk of memory, and commit/reserve the section at one time, which means memory in this section will be initialized to 0.
			//Fourth is the protection level, and PAGE_READWRITE allows us to read and write to a dynamically sized section like this one :D.
			render_state.memory = VirtualAlloc(0, render_state_size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
			render_state.bitmapinfo.bmiHeader.biSize = sizeof(render_state.bitmapinfo.bmiHeader);
			render_state.bitmapinfo.bmiHeader.biWidth = render_state.width;
			render_state.bitmapinfo.bmiHeader.biHeight = render_state.height;
			render_state.bitmapinfo.bmiHeader.biPlanes = 1;
			render_state.bitmapinfo.bmiHeader.biBitCount = 32; //32 since we are running on the size of a 32 bit integer.
			render_state.bitmapinfo.bmiHeader.biCompression = BI_RGB;
		} break;
		default: {
			result = DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	return result;
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) 
{
	ShowCursor(false);
	WNDCLASS window_class = {};//Declare window
	window_class.style = CS_HREDRAW | CS_VREDRAW;
	window_class.lpszClassName = L"Game Window Class";
	window_class.lpfnWndProc = window_callback;//This is the callback function that we use to receive communication from the window.
	RegisterClass(&window_class);
	HWND window = CreateWindow(window_class.lpszClassName, L"Pong by Jason Harris - ToadNamedDuck", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, 0, 0, hInstance, 0);
	{
		//Set fullscreen
		SetWindowLong(window, GWL_STYLE, GetWindowLong(window, GWL_STYLE) & ~WS_OVERLAPPEDWINDOW);
		MONITORINFO mi = { sizeof(mi) };
		GetMonitorInfo(MonitorFromWindow(window, MONITOR_DEFAULTTOPRIMARY), &mi);
		SetWindowPos(window, HWND_TOP, mi.rcMonitor.left, mi.rcMonitor.top, mi.rcMonitor.right - mi.rcMonitor.left, mi.rcMonitor.bottom - mi.rcMonitor.top, SWP_NOOWNERZORDER|SWP_FRAMECHANGED);
	}
	HDC device_context = GetDC(window);

	Input input = {};

	float delta_time = (float)1/MAXFPS;//This is the delta time for when to render. Keeps game logic consistent on dif systems.
	//To start calculating that, we need to get the CPU time at the start and end of the frame.
	LARGE_INTEGER frame_begin_time;
	QueryPerformanceCounter(&frame_begin_time);

	float performance_frequency;
	{
		LARGE_INTEGER perf;
		QueryPerformanceFrequency(&perf);
		performance_frequency = (float)(perf.QuadPart);
	}

	while (running) {
		//This is where we create our game loop!
		if (delta_time < (float)(1 / MAXFPS)) {
			std::this_thread::sleep_for(std::chrono::microseconds((int)(1000000 * (float)(1 / MAXFPS - delta_time))));
		}
		//Get input
		MSG message;
		for (int i = 0; i < BUTTON_COUNT; i++) {
			input.buttons[i].changed = false;
		}

		while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
			
			switch (message.message)
			{
				case WM_KEYUP:	//Test for key presses
				case WM_KEYDOWN:{
					u32 vk_code = (u32)message.wParam;//Get us which key is pressed.
					bool is_down = ((message.lParam & (1 << 31)) == 0);//This is supposed to check if the key is down or up.
					//The (1 << 31) is a left bit shift to check the position in the signed 32 bit lParam where the 1 would go in the new mask we create by bit shifting.
					//If the corresponding position in lParam is 0, then the key is down.

//This macro is a helper that allows up to quickly set the down state and the change state of buttons we pass into it. Check for is_down prevents OS multipresses from being read.
#define process_button(b, vk)\
case vk:{\
input.buttons[b].changed = is_down != input.buttons[b].is_down;\
input.buttons[b].is_down = is_down;\
}break;\

					switch (vk_code) {
						process_button(BUTTON_UP, VK_UP);
						process_button(BUTTON_DOWN, VK_DOWN);
						process_button(BUTTON_LEFT, VK_LEFT);
						process_button(BUTTON_RIGHT, VK_RIGHT);
						process_button(BUTTON_ENTER, VK_RETURN);
						process_button(BUTTON_W, 'W');
						process_button(BUTTON_S, 'S');
						process_button(BUTTON_ESCAPE, VK_ESCAPE);
					}
					} break;
				default: {
					TranslateMessage(&message);//Translate the message
					DispatchMessage(&message);//Dispatch the message to our callback!
				}
			}
		}

		//Simulate
		simulate_game(&input, delta_time);
		//Render
		// 
		//To render, we need to get the buffer, fill it, and send it to the OS to handle.
		//To get the proper buffer, we need to get the proper size of the window, which the player could change, so to properly handle that, get it every cycle from messages sent to window.
		//That happens in the proc on a wm size message case.
		StretchDIBits(device_context, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmapinfo, DIB_RGB_COLORS, SRCCOPY);

		//End of Frame
		LARGE_INTEGER frame_end_time;
		QueryPerformanceCounter(&frame_end_time);
		delta_time = (float)(frame_end_time.QuadPart - frame_begin_time.QuadPart) / performance_frequency;
		frame_begin_time = frame_end_time;
	}
}