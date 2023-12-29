internal void 
simulate_game(Input* input) {
	clear_screen(0xc285d3);
	if (input->buttons[BUTTON_UP].is_down) {
		draw_rect(0, 0, 12, 17, 0x00ff00);//3:53
	}
	draw_rect(19, -20, 5, 5, 0x00ff00);
	draw_rect(-15, 25, 8, 4, 0x00ff00);
}