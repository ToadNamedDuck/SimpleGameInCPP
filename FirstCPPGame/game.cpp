#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_pos, player_derPos;//Player position vertical


internal void 
simulate_game(Input* input, float dt) {
	clear_screen(0x881253);
	draw_rect(0, 0, 85, 45, 0x00cb00);

	float player_ddp = 0.f;//Derivitive of the derivitive of the player position. Friction.

	if (is_down(BUTTON_UP)) player_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_ddp -= 2000;

	player_ddp -= player_derPos * 10.f;

	player_pos = player_pos + player_derPos * dt + player_ddp * dt * dt * .5f;
	player_derPos = player_derPos + player_ddp * dt;


	draw_rect(0, 0, 1, 1, 0xffffff);//DRAW AFTER INCREMENTATION

	draw_rect(80, 0, 2.5, 12, 0x5522ff);
	draw_rect(-80, player_pos, 2.5, 12, 0x5522ff);

}