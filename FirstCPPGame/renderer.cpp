void render_background() {

	unsigned int* pixel = (unsigned int*)render_state.memory;//We are going through the memory, which is the same size as window, and setting "pixel" to the current value of the spot in memory
	for (int y = 0; y < render_state.height; y++) {//What this is doing is defining which "row" we are on
		for (int x = 0; x < render_state.width; x++) {//Select our "column"
			*pixel++ = 0xc285d3;//set the individual spot in memory to this value so we can display its value (color)
		}
	}
}