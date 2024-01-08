#define is_down(b) input->buttons[b].is_down//If the button is down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)//If the button is down and it has changed since the last frame.
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)//If the button is not down and has changed since the last frame.

float player_1_pos, player_1_derPos, player_2_pos, player_2_derPos, friction = 10;
float arena_half_size_x = 85.f, arena_half_size_y = 45.f;
float player_half_size_x = 2.5, player_half_size_y = 12;
float ball_pos_x, ball_pos_y, ball_derPos_x = 130.f, ball_derPos_y, ball_half_size_x = 1, ball_half_size_y = 1;

int player_1_score, player_2_score;

//Derivative of position is velocity. DP
//Derivative of derivative of position is acceleration.
//Input increases acceleration value, and *friction* can be applied by subtracting from acceleration velocity x friction.

internal void
simulate_player(float *pos, float *vel, float accel, float frict, float dt) {
	accel -= *vel * frict;
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

//This is a function to do Axis-Aligned Bounding Box collision tests (here, for the ball)
internal bool
aabb_vs_aabb(float p1_x, float p1_y, float p1_hs_x, float p1_hs_y,
	float p2_x, float p2_y, float p2_hs_x, float p2_hs_y) {
	return (p1_x + p1_hs_x > p2_x - p2_hs_x &&
		p1_x - p1_hs_x < p2_x + p2_hs_x&&
		p1_y + p1_hs_y > p2_y - p2_hs_y&&
		p1_y + p1_hs_y < p2_y + p2_hs_y);
}

//Implement modes to implement menu, and separate the menu from gameplay.
enum Gamemode {
	GM_MENU,//Making a variable from an enum type auto-selects the first member, so I had to change the order of the enum.
	GM_GAMEPLAY,

	GM_QUIT,
	GM_COUNT
};

Gamemode current_gamemode;
int hot_button;
bool enemy_is_ai;

enum pauseMenu {
	PM_RESUME,
	PM_RESTART,
	PM_RETURN,

	PM_COUNT
};
pauseMenu pause_menu;
bool isPaused;
int pause_button;

internal void 
simulate_game(Input* input, float dt) {
	draw_rect(0, 0, arena_half_size_x, arena_half_size_y, 0x00cb00);//Arena
	draw_arena_borders(arena_half_size_x, arena_half_size_y, 0x881253);
	float player_1_ddp = 0.f, player_2_ddp = 0.f;

	switch (current_gamemode) {
	case GM_GAMEPLAY: {
		if(!isPaused)
		{
			if (pressed(BUTTON_ESCAPE)) {
				isPaused = !isPaused;
			}
			if (is_down(BUTTON_W)) player_1_ddp += 2000;
			if (is_down(BUTTON_S)) player_1_ddp -= 2000;
			if (!enemy_is_ai) {
				if (is_down(BUTTON_UP)) player_2_ddp += 2000;
				if (is_down(BUTTON_DOWN)) player_2_ddp -= 2000;
			}
			else {
				if ((ball_pos_y - player_2_pos) > 2.f || (ball_pos_y - player_2_pos) < 2.f) {
					player_2_ddp = ((ball_pos_y - player_2_pos) * 750);
					if (player_2_ddp > 1600) player_2_ddp = 1600;
					if (player_2_ddp < -1600) player_2_ddp = -1600;
				}
				if (ball_pos_x < 0 && (ball_derPos_y > 40 | ball_derPos_y < -40)) {
					player_2_ddp *= -.75f;
				}
			}

			simulate_player(&player_1_pos, &player_1_derPos, player_1_ddp, friction, dt);
			simulate_player(&player_2_pos, &player_2_derPos, player_2_ddp, friction, dt);

			//Simulate Ball
			{
				ball_pos_x += ball_derPos_x * dt;
				ball_pos_y += ball_derPos_y * dt;

				if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size_x, ball_half_size_y, 80, player_2_pos, player_half_size_x, player_half_size_y))
				{
					ball_pos_x = 80 - player_half_size_x - ball_half_size_x;
					ball_derPos_x *= -1;
					ball_derPos_y = (ball_pos_y - player_2_pos) * 2 + player_2_derPos * .75/*Change velocity by dif in pos of player and ball. Can also add a fraction of player velocity.*/;
					PlaySound(L"bounce", NULL, SND_RESOURCE | SND_ASYNC);
				}
				else if (aabb_vs_aabb(ball_pos_x, ball_pos_y, ball_half_size_x, ball_half_size_y, -80, player_1_pos, player_half_size_x, player_half_size_y))
				{
					ball_pos_x = -80 + player_half_size_x + ball_half_size_x;
					ball_derPos_x *= -1;
					ball_derPos_y = (ball_pos_y - player_1_pos) * 2 + player_1_derPos * .75;
					PlaySound(L"bounce", NULL, SND_RESOURCE | SND_ASYNC);
				}
				//Check for top and bottom of arena collision.
				if (ball_pos_y + ball_half_size_y > arena_half_size_y)
				{
					ball_pos_y = arena_half_size_y - ball_half_size_y;
					ball_derPos_y *= -1;
					PlaySound(L"bounce", NULL, SND_RESOURCE | SND_ASYNC);
				}
				else if (ball_pos_y - ball_half_size_y < -arena_half_size_y)
				{
					ball_pos_y = -arena_half_size_y + ball_half_size_y;
					ball_derPos_y *= -1;
					PlaySound(L"bounce", NULL, SND_RESOURCE | SND_ASYNC);
				}
				if (ball_pos_x + ball_half_size_x > arena_half_size_x)
				{	//Right and Left Arena check - goal
					//score point
					//Reset ball.
					player_2_score += 1;
					ball_pos_x = 0, ball_pos_y = 0, ball_derPos_y = 0;
					PlaySound(L"point", NULL, SND_RESOURCE | SND_ASYNC);
				}
				else if (ball_pos_x - ball_half_size_x < -arena_half_size_x)
				{
					player_1_score += 1;
					ball_pos_x = 0, ball_pos_y = 0, ball_derPos_y = 0;
					PlaySound(L"point", NULL, SND_RESOURCE | SND_ASYNC);
				}
			}

			draw_number(player_2_score, -10, 40, 1.f, 0xbbffbb);
			draw_number(player_1_score, 10, 40, 1.f, 0xbbffbb);

			draw_rect(ball_pos_x, ball_pos_y, ball_half_size_x, ball_half_size_y, 0xffffff);//Ball
			draw_rect(-80, player_1_pos, player_half_size_x, player_half_size_y, 0x5522ff);//Right
			draw_rect(80, player_2_pos, player_half_size_x, player_half_size_y, 0x5522ff);//Left
		}
		else {
			draw_text("PAUSED", -15, 30, 1, 0xffbf00);
			if (pressed(BUTTON_ESCAPE)) {
				pause_button = 0;
				isPaused = !isPaused;
			}
			if (released(BUTTON_LEFT)) {
				PlaySound(L"move", NULL, SND_RESOURCE | SND_ASYNC);
				pause_button -= 1;
				if (pause_button < 0) {
					pause_button = PM_COUNT - 1;
				}
			}
			if (released(BUTTON_RIGHT)) {
				PlaySound(L"move", NULL, SND_RESOURCE | SND_ASYNC);
				pause_button += 1;
				if (pause_button >= PM_COUNT) {
					pause_button = 0;
				}
			}
			switch (pause_button) {
				case 0: {
					draw_text("RESUME", -40, 0, .75, 0xf0f000);
					draw_text("RESTART", 12, 0, .75, 0xffbf00);
					draw_text("RETURN TO MAIN MENU", -40, -15, .75, 0xffbf00);
				}break;
				case 1: {
					draw_text("RESUME", -40, 0, .75, 0xffbf00);
					draw_text("RESTART", 12, 0, .75, 0xf0f000);
					draw_text("RETURN TO MAIN MENU", -40, -15, .75, 0xffbf00);
				}break;
				case 2: {
					draw_text("RESUME", -40, 0, .75, 0xffbf00);
					draw_text("RESTART", 12, 0, .75, 0xffbf00);
					draw_text("RETURN TO MAIN MENU", -40, -15, .75, 0xf0f000);
				}break;
			}
			if (released(BUTTON_ENTER)) {
				PlaySound(L"select", NULL, SND_RESOURCE | SND_SYNC);
				switch (pause_button) {
					case 0:{//Resume
						pause_button = 0;
						isPaused = !isPaused;
					}break;
					case 1:{//Reset Current Match
						pause_button = 0;
						player_1_pos = 0.f;
						player_2_pos = 0.f;
						player_1_derPos = 0.f;
						player_2_derPos = 0.f;
						player_1_ddp = 0.f;
						player_2_ddp = 0.f;
						ball_pos_x = 0.f;
						ball_pos_y = 0.f;
						ball_derPos_x = 130.f;
						ball_derPos_y = 0.f;
						player_1_score = 0;
						player_2_score = 0;
						isPaused = !isPaused;
					}break;
					case 2: {//Return to menu
						pause_button = 0;
						player_1_pos = 0.f;
						player_2_pos = 0.f;
						player_1_derPos = 0.f;
						player_2_derPos = 0.f;
						player_1_ddp = 0.f;
						player_2_ddp = 0.f;
						ball_pos_x = 0.f;
						ball_pos_y = 0.f;
						ball_derPos_x = 130.f;
						ball_derPos_y = 0.f;
						player_1_score = 0;
						player_2_score = 0;
						isPaused = !isPaused;
						current_gamemode = GM_MENU;
					}break;
				}
			}
		}
	}break;
	case GM_MENU: {
		if (released(BUTTON_LEFT)) {
			PlaySound(L"move", NULL, SND_RESOURCE | SND_ASYNC);
			hot_button -= 1;
			if (hot_button < 0) {
				hot_button = GM_COUNT - 1;
			}
		}
		if (released(BUTTON_RIGHT)) {
			PlaySound(L"move", NULL, SND_RESOURCE | SND_ASYNC);
			hot_button += 1;
			if (hot_button >= GM_COUNT) {
				hot_button = 0;
			}
		}
		if (released(BUTTON_ENTER)) {
			if (hot_button != GM_COUNT - 1) {
				PlaySound(L"select", NULL, SND_RESOURCE | SND_SYNC);
				current_gamemode = GM_GAMEPLAY;
				enemy_is_ai = hot_button ? 0 : 1;//If hot_button isn't 0, then enemy AI is 0 - As in, the second button is to play against someone else in your house.
			}
			else {
				running = false;
			}
		}

		switch (hot_button) {
			case 0: 
			{
				{
					draw_rect(20, 0, 10, 10, 0xff0000);//Multiplayer
					draw_rect(-20, 0, 10, 10, 0xffbf00);
					draw_rect(-20, 0, 8.5, 8.5, 0xff1010);//Single Player
					draw_rect(75, -40, 10, 5, 0xff0000);//Quit
				}
			}break;
			case 1:
			{
				draw_rect(20, 0, 10, 10, 0xffbf00);
				draw_rect(20, 0, 8.5, 8.5, 0xff1010);
				draw_rect(-20, 0, 10, 10, 0xff0000);
				draw_rect(75, -40, 10, 5, 0xff0000);
			}break;
			case 2:
			{
				draw_rect(20, 0, 10, 10, 0xff0000);
				draw_rect(-20, 0, 10, 10, 0xff0000);
				draw_rect(75, -40, 10, 5, 0xffbf00);
				draw_rect(75, -40, 9, 4, 0xff1010);
			}break;
			//Can add more cases for more buttons down the road.
		}
		draw_text("PONG", -20, 40, 2, 0xffbf00);
		draw_text("BY TOADNAMEDDUCK", -39.5, 25, .85, 0xffbf00);
		draw_text("SOLO", -27, 2, .75, 0xffbf00);
		draw_text("VS", 17, 2, .75, 0xffbf00);
		draw_text("QUIT", 68.5, -38, .75, 0xffbf00);
	}break;
	}
}