#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_TITLE "02 Close Window"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
};

void game_cleanup(struct Game *game, int exit_status);
bool sdl_initialize(struct Game *game);

int main() {
  struct Game game = {
      .window = NULL,
      .renderer = NULL,
  };

  if (sdl_initialize(&game)) {
    game_cleanup(&game, EXIT_FAILURE);
  }

  SDL_Event event;
  while (true) {
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        game_cleanup(&game, EXIT_SUCCESS);
        break;
      case SDL_KEYDOWN:
        switch (event.key.keysym.scancode) {
        case SDL_SCANCODE_ESCAPE:
          game_cleanup(&game, EXIT_SUCCESS);
          break;
        default:
          break;
        }
      default:
        break;
      }
    }
    SDL_RenderClear(game.renderer);
    SDL_RenderPresent(game.renderer);
    SDL_Delay(16);
  }

  game_cleanup(&game, EXIT_SUCCESS);
  return 0;
};

void game_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  SDL_Quit();
  exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return true;
  };

  game->window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                  SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (!game->window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return true;
  }

  game->renderer =
      SDL_CreateRenderer(game->window, -1, SDL_RENDERER_ACCELERATED);
  if (!game->renderer) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return true;
  }

  return false;
};
