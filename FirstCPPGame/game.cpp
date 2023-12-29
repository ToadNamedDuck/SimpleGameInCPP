#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_pos = 0.f;//Player position


internal void 
simulate_game(Input* input) {
	clear_screen(0xc285d3);
	if (pressed(BUTTON_UP)) {
		player_pos += 1.f;//When the player presses up, increment the position up
	}
	draw_rect(0, player_pos, 2, 2, 0x00ff00);//DRAW AFTER INCREMENTATION
	draw_rect(19, -20, 5, 5, 0x00ff00);
	draw_rect(-15, 25, 8, 4, 0x00ff00);
}