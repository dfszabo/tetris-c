#include "../includes/constants.h"
#include "../includes/graphics.h"
#include "../includes/tetris.h"
#include <stdlib.h>
#include <unistd.h>

extern SDL_Renderer *renderer;

int is_game_running;

Action action = NONE;

uint round_counter = 0;
uint score_accumlator = 0;
extern uint run_count;

void init_game(Tetris *tetris) {}

void handle_io() {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
  case SDL_QUIT:
    is_game_running = FALSE;
  case SDL_KEYDOWN:
    if (event.key.keysym.sym == SDLK_ESCAPE)
      is_game_running = FALSE;
    break;
  default:
    break;
  }
}

void try_to_move_down(Tetris *tetris) {
  if (!move_piece_down(tetris)) {
    uint kind = tetris->next_piece_kind;

    add_current_piece(tetris);

    if (!does_piece_fit(tetris, kind, 0, 0, 5)) {
      round_counter++;
      score_accumlator += tetris->score;

      *tetris = new_tetris();
      if (round_counter >= run_count)
        is_game_running = false;
    }

    find_and_remove_filled_lines(tetris);

    tetris->curr_piece = (Piece){.kind = kind, .rotation = 0, .x = 0, .y = 5};
    tetris->next_piece_kind = rand() % 7;
    tetris->bot_target_piece =
        (Piece){.kind = 100, .rotation = 0, .x = 0, .y = 0};

    tetris->score++;
  }
}

void update(Tetris *tetris, uint *params) {
  static uint loop_counter = 0;

  if (loop_counter++ > 20) {
    loop_counter = 0;
    action = MOVE_DOWN;
  } else
    action = bot(tetris, params);

  bool move_succeeded = true;
  switch (action) {
  case MOVE_LEFT:
    move_succeeded = move_piece_left(tetris);
    break;
  case MOVE_RIGHT:
    move_succeeded = move_piece_right(tetris);
    break;
  case ROTATE:
    move_succeeded = rotate_piece(tetris);
    break;
  default:
    break;
  }

  // FIXME: move_succeeded is needed because the BOT not aware of yet
  // that it cannot make the move it want to left to right since in the
  // calculation it is not taken into account that something might block
  // the moving from left to right or even the rotation
  if (action == MOVE_DOWN || move_succeeded == false)
    try_to_move_down(tetris);
}

void draw(Tetris *tetris) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  add_current_piece(tetris);
  for (int row = 0; row < BOARD_HEIGHT; row++)
    for (int col = 0; col < BOARD_WIDTH; col++) {
      SDL_Rect square = {col * BLOCK_SIZE, row * BLOCK_SIZE, BLOCK_SIZE,
                         BLOCK_SIZE};

      if (tetris->board[row][col] != 0)
        SDL_SetRenderDrawColor(renderer, 128, 255 / 6 * tetris->board[row][col],
                               55 / 6 * tetris->board[row][col], 255);
      else
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

      SDL_RenderFillRect(renderer, &square);
    }
  remove_current_piece(tetris);

  SDL_RenderPresent(renderer);
}

int tetris_main(uint *params, bool with_gui) {
  if (with_gui)
    is_game_running = init_graphics();
  else
    is_game_running = true;

  Tetris tetris = new_tetris(run_count);

  round_counter = 0;
  score_accumlator = 0;

  while (is_game_running) {
    if (with_gui)
      handle_io();
    update(&tetris, params);
    if (with_gui) {
      draw(&tetris);
      usleep(30000);
    }
  }

  if (with_gui)
    quit_graphics();
  uint ret = score_accumlator / run_count;
  printf("score = %d ", ret);

  return ret;
}

// Using the best params found yet
int single_run(bool with_gui) {
  uint params[6] = {29185, 186201, 517715, 296899, 90791, 512512};
  tetris_main(params, with_gui);
  return 0;
}

int genetic_main();

int main(int argc, char *argv[]) {
  bool with_gui = false;
  bool genetic = false;

  for (int i = 1; i < argc; i++) {
    if (0 == strcmp(argv[i], "-genetic"))
      genetic = true;
    else if (0 == strcmp(argv[i], "-gui"))
      with_gui = true;
  }

  if (genetic)
    return genetic_main();
  else
    return single_run(with_gui);
}
