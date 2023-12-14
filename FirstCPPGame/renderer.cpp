void render_background() {

	unsigned int* pixel = (unsigned int*)render_state.memory;//We are going through the memory, which is the same size as window, and setting "pixel" to the current value of the spot in memory
	for (int y = 0; y < render_state.height; y++) {//What this is doing is defining which "row" we are on
		for (int x = 0; x < render_state.width; x++) {//Select our "column"
			*pixel++ = 0xc285d3;//set the individual spot in memory to this value so we can display its value (color)
		}
	}
}

void clear_screen(unsigned int color) {//reset the pixels to a provided color.
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

void draw_rect(int x0, int y0, int x1, int y1, unsigned int color) {//Now we need to offset a region to draw a rectangle in.
	for (int y = y0; y < y1; y++) {//Starting y is y0, end is y1
		unsigned int* pixel = (unsigned int*)render_state.memory + x0 + y*render_state.width;//We offset the pixel by the current x, and then y*the width of the window.
		for (int x = x0; x < x1; x++) {//starting x is x0, and is x1
			*pixel++ = color;//Set the pixels in this region to the provided color.
		}
	}
}