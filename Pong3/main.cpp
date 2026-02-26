#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdio>

// try to eliminate magic numbers and create named constants const or constexpr for each hard coded value.

// obviously I would remove commented code if this was a release application this is a portfolio project.

struct Game {
	float screenWidth;
	float screenHeight;
	SDL_Window* window;
	SDL_Renderer* renderer;
	TTF_Font* font;
	SDL_FRect paddle;
	SDL_FRect paddle2;
	SDL_FRect ball;
	float ballSpeedX;
	float ballSpeedY;
	int score1;
	int score2;
	bool running;
};

void render(Game& game) {
	SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 255);
	SDL_RenderClear(game.renderer);
	char scoreText1[16];
	char scoreText2[16];
	snprintf(scoreText1, sizeof(scoreText1), "%d", game.score1);
	snprintf(scoreText2, sizeof(scoreText2), "%d", game.score2);

	SDL_Color white = { 255, 255, 255, 255 };

	SDL_Surface* surface1 = TTF_RenderText_Blended(game.font, scoreText1, 0, white);
	SDL_Texture* texture1 = SDL_CreateTextureFromSurface(game.renderer, surface1);
	SDL_FRect scoreRect1 = { game.screenWidth / 4, 20, (float)surface1->w, (float)surface1->h };
	SDL_RenderTexture(game.renderer, texture1, nullptr, &scoreRect1);
	SDL_DestroySurface(surface1);
	SDL_DestroyTexture(texture1);


	SDL_Surface* surface2 = TTF_RenderText_Blended(game.font, scoreText2, 0, white);
	SDL_Texture* texture2 = SDL_CreateTextureFromSurface(game.renderer, surface2);
	SDL_FRect scoreRect2 = { game.screenWidth * 3 / 4 - 20, 20, (float)surface2->w, (float)surface2->h };
	SDL_RenderTexture(game.renderer, texture2, nullptr, &scoreRect2);
	SDL_DestroySurface(surface2);
	SDL_DestroyTexture(texture2);

	SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(game.renderer, &game.paddle);
	SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(game.renderer, &game.paddle2);
	SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(game.renderer, &game.ball);
	SDL_FRect centerLine = { game.screenWidth / 2 - 2, 0, 4, (float)game.screenHeight};
	SDL_RenderFillRect(game.renderer, &centerLine);
	SDL_RenderPresent(game.renderer);
}

int main(int argc, char* argv[]) {
	//const int SCREEN_WIDTH = 800;
	//const int SCREEN_HEIGHT = 600;
	Game game;
	game.screenWidth = 800;
	game.screenHeight = 600;
	const float PADDLE_WIDTH = 20;
	const float PADDLE_HEIGHT = 100;
	const float BALL_SIZE = 10;
	const float BALL_SPEED = 4;
	const float PADDLE_SPEED = 5;
	const float AI_SPEED = 4;
	const int FRAME_DELAY = 8;
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	game.font = TTF_OpenFont("C:\\Windows\\Fonts\\arial.ttf", 36);	
	if (!game.font) {
		SDL_Log("Font failed: %s", SDL_GetError());
		return 1;
	}
	 game.window = SDL_CreateWindow("Pong", game.screenWidth, game.screenHeight, SDL_WINDOW_RESIZABLE);
	 game.renderer = SDL_CreateRenderer(game.window, nullptr);

	 game.running = true;
	 game.paddle = { 0, game.screenHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	 game.paddle2 = { game.screenWidth - PADDLE_WIDTH, game.screenHeight / 2 - PADDLE_HEIGHT / 2, PADDLE_WIDTH, PADDLE_HEIGHT };
	 game.ball = { game.screenWidth / 2, game.screenHeight /2, BALL_SIZE, BALL_SIZE };
	 game.ballSpeedX = BALL_SPEED;
	 game.ballSpeedY = BALL_SPEED;
	 game.score1 = 0;
	 game.score2 = 0;
	while (game.running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) game.running = false;

		}
		const bool* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_W]) game.paddle.y -= PADDLE_SPEED;
		if (keys[SDL_SCANCODE_S]) game.paddle.y += PADDLE_SPEED;
		if (game.paddle.y < 0) game.paddle.y = 0;
		if (game.paddle.y + game.paddle.h > game.screenHeight) game.paddle.y = game.screenHeight - game.paddle.h;
		// AI paddle control for player two
		
		if (game.paddle2.y + game.paddle2.h / 2 < game.ball.y) game.paddle2.y += AI_SPEED;
		if (game.paddle2.y + game.paddle2.h / 2 > game.ball.y) game.paddle2.y -= AI_SPEED;
		if (keys[SDL_SCANCODE_P]) game.paddle2.y -= PADDLE_SPEED; //if you want player two to have controls.
		if (keys[SDL_SCANCODE_L]) game.paddle2.y += PADDLE_SPEED; //if you want player two to have controls.
		if (game.paddle2.y < 0) game.paddle2.y = 0;
		if (game.paddle2.y + game.paddle2.h > game.screenHeight) game.paddle2.y = game.screenHeight - game.paddle2.h;
		game.ball.x += game.ballSpeedX;
		game.ball.y += game.ballSpeedY;
		if (game.ball.y < 0 || game.ball.y + game.ball.h > game.screenHeight) game.ballSpeedY = -game.ballSpeedY;

		
			if (game.ball.x < 0) {
			game.ball.x = game.screenWidth / 2;
			game.ball.y = game.screenHeight /2;
			game.ballSpeedX = BALL_SPEED;
			game.ballSpeedY = BALL_SPEED;
			game.score2++;
		}
		if (game.ball.x + game.ball.w > game.screenWidth) {
			game.ball.x = game.screenWidth / 2;
			game.ball.y = game.screenHeight / 2;
			game.ballSpeedX = -BALL_SPEED;
			game.ballSpeedY = BALL_SPEED;
			game.score1++;
		}
		
		// These two lines will get the ball bouncing period. Basic Wall Collision.
		// if (ball.y < 0 || ball.y + ball.h > 600) ballSpeedY = -ballSpeedY; // This line handles top and bottom wall collison
		//if (ball.x < 0 || ball.x + ball.w > 800) ballSpeedX = -ballSpeedX; // This line handles left and right wall collison -- The paddle collison logic handles this now for each of it's respective paddles
		

		//Paddle Collison Logic
		// I encountered a problem where the ball would be under, say the right paddle in this instance
		// and it would get stuck under the paddle and the game would continue running with the paddle
		// underneath paddle2's paddle and the ball would move up and down with the right paddle
		// causing the game to continue playing but not reset. A line that acts a guard in the paddle collison logic
		// was added to prevent this from ever happening. (Heh, unless I find another bug later on.)
		if (game.ball.x < game.paddle.x + game.paddle.w && game.ball.y + game.ball.h > game.paddle.y && game.ball.y < game.paddle.y + game.paddle.h) {
			game.ballSpeedX = -game.ballSpeedX;
			game.ball.x = game.paddle.x + game.paddle.w; // this line keeps the ball's x from ever overlapping with the paddles right edge.
		}

		if (game.ball.x + game.ball.w > game.paddle2.x && game.ball.y + game.ball.h > game.paddle2.y && game.ball.y < game.paddle2.y + game.paddle2.h) {
			game.ballSpeedX = -game.ballSpeedX;
			game.ball.x = game.paddle2.x - game.ball.w; // this line keeps the ball's x from ever overlapping with the paddle2's left edge(whether it is AI controlled or not this is just paddle logic.)
		}



		render(game);
		// The loop runs at an uncapped framerate on the graphics card.
		// This line will make it so that it runs at 60fps which 1 frame of 
		// game time is 16ms realtime to advance the games logic one frame of motion.
		//  16ms = 60 fps; 8ms = 120fps; 4ms approx 250 fps; 2ms = 480hz++ too fast most likely will cause issues for this game.
		SDL_Delay(FRAME_DELAY);// the line the actually sets a hard cap on the games frame rate so it caps at 120 fps, but of course could run less.
	}
	return 0;

}