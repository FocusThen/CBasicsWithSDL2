#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define WINDOW_TITLE "07 Player Sprite"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define IMAGE_FLAGS IMG_INIT_PNG
#define TEXT_SIZE 80

struct Game {
  SDL_Window *window;
  SDL_Renderer *renderer;
  SDL_Texture *background;
  TTF_Font *text_font;
  SDL_Color text_color;
  SDL_Rect text_rect;
  SDL_Texture *text_image;
  int text_xvel;
  int text_yvel;
  SDL_Texture *sprite_image;
  SDL_Rect sprite_rect;
  int sprite_vel;
  const Uint8 *keystate;
};

void game_cleanup(struct Game *game, int exit_status);
bool load_media(struct Game *game);
bool sdl_initialize(struct Game *game);
void text_update(struct Game *game);
void sprite_update(struct Game *game);

int main() {
  struct Game game = {
      .window = NULL,
      .renderer = NULL,
      .background = NULL,
      .text_font = NULL,
      .text_color = {255, 255, 255, 255},
      .text_rect = {100, 0, 0, 0},
      .text_image = NULL,
      .text_xvel = 3,
      .text_yvel = 3,
      .sprite_image = NULL,
      .sprite_rect = {0, 0, 0, 0},
      .sprite_vel = 5,
      .keystate = SDL_GetKeyboardState(NULL),
  };

  if (sdl_initialize(&game)) {
    game_cleanup(&game, EXIT_FAILURE);
  }

  if (load_media(&game)) {
    game_cleanup(&game, EXIT_FAILURE);
  }

  while (true) {
    SDL_Event event;
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
        case SDL_SCANCODE_SPACE:
          SDL_SetRenderDrawColor(game.renderer, rand() % 256, rand() % 256,
                                 rand() % 256, 255);
          break;
        default:
          break;
        }
      default:
        break;
      }
    }

    text_update(&game);

    sprite_update(&game);

    SDL_RenderClear(game.renderer);

    SDL_RenderCopy(game.renderer, game.background, NULL, NULL);

    SDL_RenderCopy(game.renderer, game.text_image, NULL, &game.text_rect);

    SDL_RenderCopy(game.renderer, game.sprite_image, NULL, &game.sprite_rect);

    SDL_RenderPresent(game.renderer);

    SDL_Delay(16);
  }

  game_cleanup(&game, EXIT_SUCCESS);

  return 0;
}

void game_cleanup(struct Game *game, int exit_status) {
  SDL_DestroyTexture(game->sprite_image);
  SDL_DestroyTexture(game->text_image);
  TTF_CloseFont(game->text_font);
  SDL_DestroyTexture(game->background);
  SDL_DestroyRenderer(game->renderer);
  SDL_DestroyWindow(game->window);
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();
  exit(exit_status);
}

bool sdl_initialize(struct Game *game) {
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
    return true;
  }

  int img_init = IMG_Init(IMAGE_FLAGS);
  if ((img_init & IMAGE_FLAGS) != IMAGE_FLAGS) {
    fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
    return true;
  }

  if (TTF_Init()) {
    fprintf(stderr, "Error initializing SDL_ttf: %s\n", IMG_GetError());
    return true;
  }

  game->window =
      SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED,
                       SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
  if (!game->window) {
    fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
    return true;
  }

  game->renderer = SDL_CreateRenderer(game->window, -1, 0);
  if (!game->renderer) {
    fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
    return true;
  }

  srand((unsigned)time(NULL));

  return false;
}

bool load_media(struct Game *game) {
  game->background = IMG_LoadTexture(game->renderer, "images/background.png");
  if (!game->background) {
    fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
    return true;
  }

  game->text_font = TTF_OpenFont("fonts/freesansbold.ttf", TEXT_SIZE);
  if (!game->text_font) {
    fprintf(stderr, "Error creating Font: %s\n", TTF_GetError());
    return true;
  }

  SDL_Surface *surface =
      TTF_RenderText_Blended(game->text_font, "SDL", game->text_color);
  if (!surface) {
    fprintf(stderr, "Error creating Surface: %s\n", SDL_GetError());
    return true;
  }
  game->text_rect.w = surface->w;
  game->text_rect.h = surface->h;
  game->text_image = SDL_CreateTextureFromSurface(game->renderer, surface);
  SDL_FreeSurface(surface);
  if (!game->text_image) {
    fprintf(stderr, "Error creating Texture: %s\n", SDL_GetError());
    return true;
  }

  game->sprite_image = IMG_LoadTexture(game->renderer, "images/C-logo.png");
  if (!game->sprite_image) {
    fprintf(stderr, "Error creating Texture: %s\n", IMG_GetError());
    return true;
  }

  if (SDL_QueryTexture(game->sprite_image, NULL, NULL, &game->sprite_rect.w,
                       &game->sprite_rect.h)) {
    fprintf(stderr, "Error querying Texture: %s\n", SDL_GetError());
    return true;
  }

  return false;
}

void text_update(struct Game *game) {
  game->text_rect.x += game->text_xvel;
  game->text_rect.y += game->text_yvel;
  if (game->text_rect.x + game->text_rect.w > SCREEN_WIDTH) {
    game->text_xvel = -3;
  }
  if (game->text_rect.x < 0) {
    game->text_xvel = 3;
  }
  if (game->text_rect.y + game->text_rect.h > SCREEN_HEIGHT) {
    game->text_yvel = -3;
  }
  if (game->text_rect.y < 0) {
    game->text_yvel = 3;
  }
}

void sprite_update(struct Game *game) {
  if (game->keystate[SDL_SCANCODE_LEFT] || game->keystate[SDL_SCANCODE_A]) {
    game->sprite_rect.x -= game->sprite_vel;
  }
  if (game->keystate[SDL_SCANCODE_RIGHT] || game->keystate[SDL_SCANCODE_D]) {
    game->sprite_rect.x += game->sprite_vel;
  }
  if (game->keystate[SDL_SCANCODE_UP] || game->keystate[SDL_SCANCODE_W]) {
    game->sprite_rect.y -= game->sprite_vel;
  }
  if (game->keystate[SDL_SCANCODE_DOWN] || game->keystate[SDL_SCANCODE_S]) {
    game->sprite_rect.y += game->sprite_vel;
  }
}
