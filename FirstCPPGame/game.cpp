#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_1_pos, player_1_derPos, player_2_pos, player_2_derPos;
float arena_half_size_x = 85.f, arena_half_size_y = 45.f;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_pos_x, ball_pos_y, ball_derPos_x = 100.f, ball_derPos_y, ball_half_size_x = 1, ball_half_size_y = 1;

//Derivative of position is velocity. DP
//Derivative of derivative of position is acceleration.
//Input increases acceleration value, and *friction* can be applied by

internal void
simulate_player(float *pos, float *vel, float accel, float dt) {
	accel -= *vel * 10.f;
	*pos = *pos + *vel * dt + accel * dt * dt * .5f;
	*vel = *vel + accel * dt;

	if (*pos + player_half_size_y > arena_half_size_y) {//top
		//So if these are touching, we move the player back and cut their velocity.
		*pos = arena_half_size_y - player_half_size_y;
		*vel = 0;
	}
	else if (*pos - player_half_size_y < -arena_half_size_y) {//bottom
		*pos = -arena_half_size_y + player_half_size_y;
		*vel = 0;
	}
}

internal void 
simulate_game(Input* input, float dt) {
	clear_screen(0x881253);
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00cb00);//Arena

	float player_1_ddp = 0.f, player_2_ddp = 0.f;

	if (is_down(BUTTON_W)) player_1_ddp += 2000;
	if (is_down(BUTTON_S)) player_1_ddp -= 2000;
	if (is_down(BUTTON_UP)) player_2_ddp += 2000;
	if (is_down(BUTTON_DOWN)) player_2_ddp -= 2000;

	simulate_player(&player_1_pos, &player_1_derPos, player_1_ddp, dt);
	simulate_player(&player_2_pos, &player_2_derPos, player_2_ddp, dt);

	ball_pos_x += ball_derPos_x * dt;//For whatever reason, this is causing the ball to just completely disappear from the screen. It's depressing. 8:30 ish in the video.
	ball_pos_y += ball_derPos_y * dt;

	draw_rect(ball_pos_x, ball_pos_y, ball_half_size_x, ball_half_size_y, 0xffffff);//Ball


	//Axis-Aligned Bounding Box collision test. Test for all 4 sides.
	if (ball_pos_x + ball_half_size_x > 80 - player_half_size_x &&
		ball_pos_x - ball_half_size_x < 80 + player_half_size_x &&
		ball_pos_y + ball_half_size_y > player_2_pos - player_half_size_y &&
		ball_pos_y + ball_half_size_y < player_2_pos + player_half_size_y) 
	{
		ball_pos_x = 80 - player_half_size_x - ball_half_size_x;
		ball_derPos_x *= -1;
		ball_derPos_y = (ball_pos_y - player_2_pos)*2 + player_2_derPos*.75/*Change velocity by dif in pos of player and ball. Can also add a fraction of player velocity.*/;
	}else if 
		(ball_pos_x + ball_half_size_x > -80 - player_half_size_x &&
		ball_pos_x - ball_half_size_x < -80 + player_half_size_x &&
		ball_pos_y + ball_half_size_y > player_1_pos - player_half_size_y &&
		ball_pos_y + ball_half_size_y < player_1_pos + player_half_size_y)
	{
		ball_pos_x = -80 + player_half_size_x + ball_half_size_x;
		ball_derPos_x *= -1;
		ball_derPos_y = (ball_pos_y - player_1_pos)*2 + player_1_derPos*.75;
	}

	//Check for top and bottom of arena collision.
	if (ball_pos_y + ball_half_size_y > arena_half_size_y) {
		ball_pos_y = arena_half_size_y - ball_half_size_y;
		ball_derPos_y *= -1;
	}
	else if (ball_pos_y - ball_half_size_y < -arena_half_size_y) {
		ball_pos_y = -arena_half_size_y + ball_half_size_y;
		ball_derPos_y *= -1;
	}

	if (ball_pos_x + ball_half_size_x > arena_half_size_x) {//Right and Left
		//Right side scores point
		//Reset ball.
		/*
		
		*/
		ball_pos_x = 0, ball_pos_y = 0, ball_derPos_y = 0;
	}
	else if (ball_pos_x - ball_half_size_x < -arena_half_size_x) {
		ball_pos_x = 0, ball_pos_y = 0, ball_derPos_y = 0;
	}

	draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0x5522ff);
	draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0x5522ff);


}