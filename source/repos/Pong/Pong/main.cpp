#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

using namespace std;

// Window dimensions - defined as constants so we can use them everywhere
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

// Paddle dimensions/configuration
const float PADDLE_WIDTH = 15.0f;
const float PADDLE_HEIGHT = 100.0f;
const float PADDLE_PADDING = 20.0f; // Distance  from screen edge
const float PADDLE_SPEED = 400.0f; // Pixels per second


// Ball Configuration
const float BALL_SIZE = 15.0f;
const float BALL_SPEED = 300.0f;

int main(int argc, char* argv[]) {
	// Intialize SDL's video subsystem
	if (!SDL_Init(SDL_INIT_VIDEO)) {
		SDL_Log("Failed to intialize SDL: %s", SDL_GetError());
		return 1;

	}
	// create the window
	SDL_Window* window = SDL_CreateWindow("Pong", 800, 600, 0);
	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		SDL_Quit();
		return 1;
	}

	// create the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
	if (!renderer) {
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}

	// Create the two paddles
	SDL_FRect paddle1 = {
		PADDLE_PADDING,                         // x position
		(WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f, // y position (centered)
		PADDLE_WIDTH,							// width
		PADDLE_HEIGHT							// height
	};

	SDL_FRect paddle2 = {
		WINDOW_WIDTH - PADDLE_PADDING - PADDLE_WIDTH, // x position
		(WINDOW_HEIGHT - PADDLE_HEIGHT) / 2.0f,       // y position
		PADDLE_WIDTH,								  // width
		PADDLE_HEIGHT								  // height
	};

	// Create the ball
	SDL_FRect ball = {
		(WINDOW_WIDTH - BALL_SIZE) / 2.0f, // Centered horizontally
		(WINDOW_HEIGHT - BALL_SIZE) / 2.0f, // Centered vertically
		BALL_SIZE, //width
		BALL_SIZE //height
	};

	// Ball velocity (pixels per second)
	float ballVelocityX = BALL_SPEED; // Moving right intially
	float ballVelocityY = BALL_SPEED; // Moving down intially

	// Timing variables
	Uint64 previousTime = SDL_GetPerformanceCounter();
	Uint64 frequency = SDL_GetPerformanceFrequency();


	// Game loop control variable
	bool running = true;

	// the game loop
	while (running) {
		// ==== Calculate Delta Time ==== //
		Uint64 currentTime = SDL_GetPerformanceCounter();
		float deltaTime = (float)(currentTime - previousTime) / (float)frequency;
		previousTime = currentTime;


		// ==== 1. Handle Events ====
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if(event.type == SDL_EVENT_QUIT){
			running = false;
		}
	}

	// ==== 2. Update ====

		// Get current keyboard state
		const bool* keyboardState = SDL_GetKeyboardState(NULL);

		// Player 1 movement (W and S keys)
		if (keyboardState[SDL_SCANCODE_W]) {
			paddle1.y -= PADDLE_SPEED * deltaTime;
		}

		if (keyboardState[SDL_SCANCODE_S]) {
			paddle1.y += PADDLE_SPEED * deltaTime;
		}

		// Player 2 Movement (Up and Down arrow keys)
		if (keyboardState[SDL_SCANCODE_UP]) {
			paddle2.y -= PADDLE_SPEED * deltaTime;
		}

		if (keyboardState[SDL_SCANCODE_DOWN]) {
			paddle2.y += PADDLE_SPEED * deltaTime;
		}


		// Clamp paddle 1 to screen bounds
		if (paddle1.y < 0) {
			paddle1.y = 0;
		}

		if (paddle1.y + PADDLE_HEIGHT > WINDOW_HEIGHT) {
			paddle1.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
		}

		// Clamp paddle 2 to screen bounds
		if (paddle2.y < 0) {
			paddle2.y = 0;
		}

		if (paddle2.y + PADDLE_HEIGHT > WINDOW_HEIGHT) {
			paddle2.y = WINDOW_HEIGHT - PADDLE_HEIGHT;
		}

		// ---- Ball Movement ----
		ball.x += ballVelocityX * deltaTime;
		ball.y += ballVelocityY * deltaTime;

		// ---- Ball Collison with Top and Bottom Walls ----
		if (ball.y <= 0) {
			ball.y = 0;
			ballVelocityY = -ballVelocityY;
		}

		if (ball.y + BALL_SIZE >= WINDOW_HEIGHT) {
			ball.y = WINDOW_HEIGHT - BALL_SIZE;
			ballVelocityY = -ballVelocityY;
		}

		// ---- Ball Collision with Paddles ----

		// Collision with paddle 1 (left paddle)
		if (ball.x <= paddle1.x + paddle1.w &&
			ball.x + BALL_SIZE >= paddle1.x &&
			ball.y + BALL_SIZE >= paddle1.y &&
			ball.y <= paddle1.y + paddle1.h &&
			ballVelocityX < 0)
		{
			ball.x = paddle1.x + paddle1.w;
			ballVelocityX = -ballVelocityX;
		}

		// Collision with paddle 2 (right paddle)
		if (ball.x + BALL_SIZE > paddle2.x &&
			ball.x <= paddle2.x + paddle2.w &&
			ball.y + BALL_SIZE >= paddle2.y &&
			ball.y <= paddle2.y + paddle2.h &&
			ballVelocityX > 0)

		{
			ball.x = paddle2.x - BALL_SIZE;
			ballVelocityX = -ballVelocityX;
		}

		// ---- Ball Out of Bounds (Scoring) ----
		if (ball.x < 0 || ball.x + BALL_SIZE > WINDOW_WIDTH) {
			// Reset ball to center
			ball.x = (WINDOW_WIDTH - BALL_SIZE) / 2.0f;
			ball.y = (WINDOW_HEIGHT - BALL_SIZE) / 2.0f;

				// Reverse direction (serve toward whoever scored)
				ballVelocityX = -ballVelocityX;
		}
	// ==== 3. Render ====
	// Clear screen to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clear the screen with the current draw color
	SDL_RenderClear(renderer);

	// Draw paddles in white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &paddle1);
	SDL_RenderFillRect(renderer, &paddle2);

	// Draw the ball

	SDL_RenderFillRect(renderer, &ball);

	// Present the rendered frame to the screen
	SDL_RenderPresent(renderer);
}

// Cleanup in reverse order of creation
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();


	
	return 0;
}