#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_1_pos, player_1_derPos, player_2_pos, player_2_derPos;//Player position vertical
float arena_half_size_x = 85.f, arena_half_size_y = 45.f;

float player_half_size_x = 2.5, player_half_size_y = 12;


internal void 
simulate_game(Input* input, float dt) {
	clear_screen(0x881253);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00cb00);//Arena

	float player_1_ddp = 0.f, player_2_ddp = 0.f;

	if (is_down(BUTTON_W)) player_1_ddp += 2000;
	if (is_down(BUTTON_S)) player_1_ddp -= 2000;
	if (is_down(BUTTON_UP)) player_2_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_2_ddp -= 2000;


	player_1_ddp -= player_1_derPos * 10.f;

	player_1_pos = player_1_pos + player_1_derPos * dt + player_1_ddp * dt * dt * .5f;
	player_1_derPos = player_1_derPos + player_1_ddp * dt;

	if (player_1_pos + player_half_size_y > arena_half_size_y) {//top
		//So if these are touching, we move the player back and cut their velocity.
		player_1_pos = arena_half_size_y - player_half_size_y;
		player_1_derPos = 0;
	}
	if (player_1_pos - player_half_size_y < -arena_half_size_y) {//bottom
		//So if these are touching, we move the player back and cut their velocity.
		player_1_pos = -arena_half_size_y + player_half_size_y;
		player_1_derPos = 0;
	}

	if (player_2_pos + player_half_size_y > arena_half_size_y) {//top
		//So if these are touching, we move the player back and cut their velocity.
		player_2_pos = arena_half_size_y - player_half_size_y;
		player_2_derPos = 0;
	}
	if (player_2_pos - player_half_size_y < -arena_half_size_y) {//bottom
		//So if these are touching, we move the player back and cut their velocity.
		player_2_pos = -arena_half_size_y + player_half_size_y;
		player_2_derPos = 0;
	}

	player_2_ddp -= player_2_derPos * 10.f;

	player_2_pos = player_2_pos + player_2_derPos * dt + player_2_ddp * dt * dt * .5f;
	player_2_derPos = player_2_derPos + player_2_ddp * dt;

	draw_rect(0, 0, 1, 1, 0xffffff);//DRAW AFTER INCREMENTATION

	draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0x5522ff);
	draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0x5522ff);


}