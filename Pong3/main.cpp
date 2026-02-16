#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

// try to eliminate magic numbers and create named constants const or constexpr for each hard coded value.

int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Pong", 800, 600, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr);

	bool running = true;
	SDL_FRect paddle = { 0, 250,20,100 };
	SDL_FRect paddle2 = { 780, 250,20,100 };
	SDL_FRect ball = { 400, 300, 10,30 };
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) running = false;

		}
		const bool* keys = SDL_GetKeyboardState(nullptr);
		if (keys[SDL_SCANCODE_W]) paddle.y -= 5;
		if (keys[SDL_SCANCODE_S]) paddle.y += 5;
		if (paddle.y < 0) paddle.y = 0;
		if (paddle.y + paddle.h > 600) paddle.y = 600 - paddle.h;
		if (keys[SDL_SCANCODE_P]) paddle2.y -= 5;
		if (keys[SDL_SCANCODE_L]) paddle2.y += 5;
		if (paddle2.y < 0) paddle2.y = 0;
		if (paddle2.y + paddle2.h > 600) paddle2.y = 600 - paddle2.h;



		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &paddle);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &paddle2);
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &ball);
	    SDL_RenderPresent(renderer);


	}
	return 0;

}