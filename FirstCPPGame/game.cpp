#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_pos = 0.f;//Player position vertical


internal void 
simulate_game(Input* input, float dt) {
	clear_screen(0x881253);
	draw_rect(0, 0, 85, 45, 0x00cb00);

	float speed = 50.f;//Units per second.
	if (is_down(BUTTON_UP)) player_pos += speed*dt;
	if (is_down(BUTTON_DOWN)) player_pos -= speed*dt;


	draw_rect(0, 0, 1, 1, 0xffffff);//DRAW AFTER INCREMENTATION

	draw_rect(80, 0, 2.5, 12, 0x5522ff);
	draw_rect(-80, 0, 2.5, 12, 0x5522ff);

}