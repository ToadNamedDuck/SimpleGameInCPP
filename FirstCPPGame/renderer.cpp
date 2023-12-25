internal void
clear_screen(u32 color) {//reset the pixels to a provided color.
	unsigned int* pixel = (unsigned int*)render_state.memory;
	for (int y = 0; y < render_state.height; y++) {
		for (int x = 0; x < render_state.width; x++) {
			*pixel++ = color;
		}
	}
}

internal void
draw_rect(int x0, int y0, int x1, int y1, u32 color) {//Now we need to offset a region to draw a rectangle in.
	x0 = clamp(0, x0, render_state.width);
	x1 = clamp(0, x1, render_state.width);
	y0 = clamp(0, y0, render_state.height);
	y1 = clamp(0, y1, render_state.height);

	for (int y = y0; y < y1; y++) {//Starting y is y0, end is y1
		unsigned int* pixel = (u32*)render_state.memory + x0 + y*render_state.width;//We offset the pixel by the current x, and then y*the width of the window.
		for (int x = x0; x < x1; x++) {//starting x is x0, and is x1
			*pixel++ = color;//Set the pixels in this region to the provided color.
		}
	}
}