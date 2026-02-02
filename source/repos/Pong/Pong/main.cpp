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

	// ==== 3. Render ====
	// Clear screen to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//Clear the screen with the current draw color
	SDL_RenderClear(renderer);

	// Draw paddles in white
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &paddle1);
	SDL_RenderFillRect(renderer, &paddle2);

	// Present the rendered frame to the screen
	SDL_RenderPresent(renderer);
}

// Cleanup in reverse order of creation
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
SDL_Quit();


	
	return 0;
}