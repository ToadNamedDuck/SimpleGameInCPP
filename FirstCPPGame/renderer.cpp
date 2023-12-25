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
draw_rect_in_pixels(int x0, int y0, int x1, int y1, u32 color) {//Now we need to offset a region to draw a rectangle in.
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

global_variable float scale_multiplier = 0.01f;
//Scale multiplier means our draw rect can accept whole numbers and convert them into a percentage of the screen in which to draw the rect.
//So 100 is a full screen, whereas 10 is 10% of the screen.

internal void
draw_rect(float x, float y, float half_size_x, float half_size_y, u32 color) {
	//To make the rect relative to screen size, we need to multiply them by the width and height of the screen - or one or the other.
	//Height will proportionally scale the rectangles. Same height but wider screens see more of the world, which in some cases, is ideal.
	//Personally I would do width and height so monitors see the same area, but alas.
	x *= render_state.height * scale_multiplier;
	half_size_x *= render_state.height * scale_multiplier;
	y *= render_state.height * scale_multiplier;
	half_size_y *= render_state.height * scale_multiplier;

	//To make "0,0" relatively centered, we need to add half of the render width and height to x and y respectively.
	x += render_state.width / 2.f;
	y += render_state.height / 2.f;

	//Change to pixels.
	int x0 = x - half_size_x;
	int x1 = x + half_size_x;
	int y0 = y - half_size_y;
	int y1 = y + half_size_y;

	draw_rect_in_pixels(x0, y0, x1, y1, color);
}