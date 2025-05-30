#include "include/common.h"
#include "include/tetris.h"

const int FIGURE_COORDS[28][4][2] = {
    {{0, 0}, {-1, 0}, {0, 1}, {-1, 1}},  /* square */
    {{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, /* original line */
    {{0, 0}, {-1, 0}, {-2, 0}, {-1, 1}}, /* T */
    {{0, 0}, {0, 1}, {-1, 0}, {-2, 0}},  /* L */
    {{0, 0}, {0, 1}, {-1, 1}, {-2, 1}},  /* J */
    {{0, 0}, {-1, 1}, {0, 1}, {-1, 2}},  /* original S */
    {{-1, 0}, {-1, 1}, {0, 1}, {0, 2}},  /* original Z */
    {{0, 0}, {-1, 0}, {0, 1}, {-1, 1}},  /* rotated clock-wise */
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},    /* laying line */
    {{0, 1}, {-1, 0}, {-1, 2}, {-1, 1}},
    {{0, 0}, {-1, 2}, {-1, 0}, {-1, 1}},
    {{0, 0}, {0, 1}, {0, 2}, {-1, 0}},
    {{0, 1}, {-1, 1}, {-1, 0}, {-2, 0}}, /* rotated S */
    {{0, 0}, {-1, 0}, {-1, 1}, {-2, 1}}, /* rotated Z */
    {{0, 0}, {-1, 0}, {0, 1}, {-1, 1}},  /* rotated 180 degrees */
    {{0, 0}, {-1, 0}, {-2, 0}, {-3, 0}}, /* original line */
    {{-1, 0}, {0, 1}, {-1, 1}, {-2, 1}},
    {{-2, 0}, {-2, 1}, {-1, 1}, {0, 1}},
    {{0, 0}, {-1, 0}, {-2, 0}, {-2, 1}},
    {{0, 0}, {-1, 1}, {0, 1}, {-1, 2}}, /* original S */
    {{-1, 0}, {-1, 1}, {0, 1}, {0, 2}}, /* original Z */
    {{0, 0}, {-1, 0}, {0, 1}, {-1, 1}}, /* rotated anticlock-wise */
    {{0, 0}, {0, 1}, {0, 2}, {0, 3}},   /* laying line */
    {{0, 0}, {0, 1}, {0, 2}, {-1, 1}},
    {{0, 0}, {0, 1}, {0, 2}, {-1, 2}},
    {{-1, 0}, {-1, 1}, {-1, 2}, {0, 2}},
    {{0, 1}, {-1, 1}, {-1, 0}, {-2, 0}}, /* rotated S */
    {{0, 0}, {-1, 0}, {-1, 1}, {-2, 1}}  /* rotated Z */
};

const int ARRAY_OF_POINTS[5] = {0, 100, 300, 700, 1500};

/**
 * This function provides a singleton instance of the `TetrisGame` structure.
 * It ensures that only one instance of `TetrisGame` exists throughout the
 * program. The `static` keyword is used to maintain the state of the instance
 * and the initialization flag across multiple calls to this function.
 *
 * @returns A pointer to the singleton instance of `TetrisGame`.
 */
TetrisGame *TetrisGameInfo() {
  static TetrisGame game_info_instance;
  static bool is_initialized = false;

  if (!is_initialized) {
    is_initialized = true;
    game_info_instance = InitTetrisGame();
  }

  return &game_info_instance;
}

void ResetTetris() {
  TetrisGame *game_info = TetrisGameInfo();
  *game_info = InitTetrisGame();
}

TetrisGame InitTetrisGame() {
  TetrisGame tetris_game;
  GameInfo_t *game_info = &tetris_game.game_info;

  tetris_game.state = START;
  tetris_game.figure_pos.x = FIELD_WIDTH / 2;
  tetris_game.figure_pos.y = SPAWN_HEIGHT;
  tetris_game.current_figure = 0;

  game_info->score = 0;
  game_info->high_score = 0;
  game_info->level = 0;

  FILE *high_score_file = fopen("logs/tetris_high_score.txt", "r");
  if (high_score_file) {
    fscanf(high_score_file, "%d", &game_info->high_score);
  } else {
    high_score_file = fopen("logs/tetris_high_score.txt", "w");
    fprintf(high_score_file, "%d", 0);
  }
  fclose(high_score_file);

  InitialField(game_info);
  SpawnFigure(game_info);

  return tetris_game;
}

void SpawnFigure(GameInfo_t *game_info) {
  TetrisGame *tetris_game = TetrisGameInfo();
  game_info->level = game_info->score / 600 % 10;

  tetris_game->current_figure = (tetris_game->current_figure + 1) % 28;
  tetris_game->figure_pos.y = SPAWN_HEIGHT;
  tetris_game->figure_pos.x = FIELD_WIDTH / 2;
}

void ScheduledFall(time_t *initial_time) {
  TetrisGame *tetris_game = TetrisGameInfo();
  time_t current_time;
  time(&current_time);
  if (difftime(current_time, *initial_time) >
      TIMEOUTS[tetris_game->game_info.level]) {
    MoveDown();
    time(initial_time);
  }
}

void MoveLeft() { MoveSideways(true); }

void MoveRight() { MoveSideways(false); }

void MoveSideways(bool to_the_left) {
  TetrisGame *tetris_game = TetrisGameInfo();
  PlayerPos *figure_pos = &tetris_game->figure_pos;
  int increment = to_the_left ? -1 : 1;
  int presence = 0;
  for (int pixel = 0; pixel < 4; pixel++) {
    int pixel_y =
        figure_pos->y + FIGURE_COORDS[tetris_game->current_figure][pixel][0];
    int pixel_x = figure_pos->x + increment +
                  FIGURE_COORDS[tetris_game->current_figure][pixel][1];
    presence |= tetris_game->game_info.field[pixel_y][pixel_x] - '.';
  }

  figure_pos->x += presence ? 0 : increment;
}

void Rotate() {
  TetrisGame *tetris_game = TetrisGameInfo();
  PlayerPos figure_pos = tetris_game->figure_pos;
  int presence = 0;
  for (int pixel = 0; pixel < 4; pixel++) {
    int pixel_y = figure_pos.y +
                  FIGURE_COORDS[(tetris_game->current_figure + FIGURE_AMOUNT) %
                                28][pixel][0];
    int pixel_x = figure_pos.x +
                  FIGURE_COORDS[(tetris_game->current_figure + FIGURE_AMOUNT) %
                                28][pixel][1];
    presence |= tetris_game->game_info.field[pixel_y][pixel_x] - '.';
  }
  if (!presence) {
    tetris_game->current_figure =
        (tetris_game->current_figure + FIGURE_AMOUNT) % 28;
  }
}

void MoveDown() {
  TetrisGame *tetris_game = TetrisGameInfo();
  if (CollisionOccured()) {
    InsertFigureIntoField();
    TerminateRows();
    if (tetris_game->figure_pos.y == SPAWN_HEIGHT)
      TetrisGameOver();
    else {
      SpawnFigure(&tetris_game->game_info);
    }
  } else {
    tetris_game->figure_pos.y++;
  }
}

int CollisionOccured() {
  TetrisGame tetris_game = *TetrisGameInfo();
  PlayerPos figure_pos = tetris_game.figure_pos;
  int presence = 0;
  for (int pixel = 0; pixel < 4; pixel++) {
    int pixel_y =
        figure_pos.y + 1 + FIGURE_COORDS[tetris_game.current_figure][pixel][0];
    int pixel_x =
        figure_pos.x + FIGURE_COORDS[tetris_game.current_figure][pixel][1];
    presence |= tetris_game.game_info.field[pixel_y][pixel_x] - '.';
  }
  return presence;
}

void InsertFigureIntoField() {
  TetrisGame *tetris_game = TetrisGameInfo();
  PlayerPos figure_pos = tetris_game->figure_pos;
  for (int pixel = 0; pixel < 4; pixel++) {
    int y = figure_pos.y + FIGURE_COORDS[tetris_game->current_figure][pixel][0];
    int x = figure_pos.x + FIGURE_COORDS[tetris_game->current_figure][pixel][1];
    tetris_game->game_info.field[y][x] = '@';
  }
}

void TerminateRows() {
  TetrisGame *tetris_game = TetrisGameInfo();
  GameInfo_t *game_info = &tetris_game->game_info;
  int rows_cleared = 0;

  for (int y = FIELD_HEIGHT - 1; y >= SPAWN_HEIGHT; y--) {
    unsigned int presence = 0xFFFFFFFF;
    for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; x++) {
      presence &= game_info->field[y][x] - '.';
    }

    if (presence) {
      rows_cleared++;
      for (int y_i = FIELD_HEIGHT - 2; y_i >= SPAWN_HEIGHT; y_i--) {
        for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; x++) {
          game_info->field[y_i + 1][x] = game_info->field[y_i][x];
        }
      }
      y++;
    }
  }
  game_info->score += ARRAY_OF_POINTS[rows_cleared];
}

void TetrisGameOver() {
  TetrisGame *tetris_game = TetrisGameInfo();
  GameInfo_t *game_info = &tetris_game->game_info;
  tetris_game->state = GAME_OVER;

  FILE *high_score_file = fopen("logs/tetris_high_score.txt", "r+");
  int high_score = 0;
  if (high_score_file) {
    fscanf(high_score_file, "%d", &high_score);
    if (game_info->score > high_score) {
      freopen("logs/tetris_high_score.txt", "w", high_score_file);
      fprintf(high_score_file, "%d", game_info->score);
    }
  } else {
    high_score_file = fopen("logs/tetris_high_score.txt", "w");
    fprintf(high_score_file, "%d", game_info->score);
  }
  fclose(high_score_file);
}
