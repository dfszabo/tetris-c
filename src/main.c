#include "../includes/constants.h"
#include "../includes/tetris.h"
#include <stdio.h>
#include <stdlib.h>

int is_game_running;

Action action = NONE;

uint round_counter = 0;
uint score_accumlator = 0;
uint run_count = 1;


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


int tetris_main(uint *params) {
  is_game_running = true;

  Tetris tetris = new_tetris();

  round_counter = 0;
  score_accumlator = 0;

  while (is_game_running)
    update(&tetris, params);

  uint ret = score_accumlator / run_count;
  printf("ret %u\n", ret);
  return ret;
}

// Using the best params found yet
int single_run() {
  uint params[6] = {1, 1, 1, 1, 1, 1};
  return tetris_main(params);
}

int main(int argc, char *argv[]) {
  return !(single_run() == 419);
}
