#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_pos_y = 0.f;//Player position vertical
float player_pos_x = 0.f;//Player position horizontal


internal void 
simulate_game(Input* input, float dt) {
	clear_screen(0xc285d3);

	float speed = 50.f;//Units per second.
	if (is_down(BUTTON_UP)) player_pos_y += speed*dt;
	if (is_down(BUTTON_DOWN)) player_pos_y -= speed*dt;
	if (is_down(BUTTON_RIGHT)) player_pos_x += speed*dt;
	if (is_down(BUTTON_LEFT)) player_pos_x -= speed*dt;


	draw_rect(player_pos_x, player_pos_y, 2, 2, 0x00ff00);//DRAW AFTER INCREMENTATION

}