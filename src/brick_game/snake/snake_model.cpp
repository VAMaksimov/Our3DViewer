#include "include/common.h"
#include "include/snake.h"

SnakeGame *SnakeGameInfo() {
  static SnakeGame game_info_instance;
  static bool is_initialized = false;

  if (!is_initialized) {
    is_initialized = true;
    game_info_instance = InitSnakeGame();
  }

  return &game_info_instance;
}

void ResetSnake() {
  SnakeGame *game_info = SnakeGameInfo();
  *game_info = InitSnakeGame();
}

/**
 * Generates pseudo-random number in range [0, 200]
 */
int GetPseudoRandomNumber() {
  static int prev = 1;
  static int curr = 1;
  static int seed = 0;

  int next = (prev + curr + seed) % (FIELD_HEIGHT * FIELD_WIDTH);
  prev = curr;
  curr = next;
  seed++;

  return next;
}

SnakeGame InitSnakeGame() {
  SnakeGame snake_game;
  GameInfo_t *game_info = &snake_game.game_info;

  snake_game.state = START;
  snake_game.snake_body.clear();
  snake_game.snake_body.push_back({START_X, START_Y});
  snake_game.snake_body.push_back({START_X, START_Y - 1});
  snake_game.snake_body.push_back({START_X, START_Y - 2});
  snake_game.snake_body.push_back({START_X, START_Y - 3});
  snake_game.new_head = {START_X, START_Y + 1};

  game_info->score = 0;
  game_info->high_score = 0;
  game_info->level = 0;

  FILE *high_score_file = fopen("logs/snake_high_score.txt", "r");
  if (high_score_file) {
    fscanf(high_score_file, "%d", &game_info->high_score);
  } else {
    high_score_file = fopen("logs/snake_high_score.txt", "w");
    fprintf(high_score_file, "%d", 0);
  }
  fclose(high_score_file);

  InitialField(game_info);
  SpawnApple(game_info);

  for (const auto &segment : snake_game.snake_body) {
    game_info->field[segment.y][segment.x] = '@';
  }
  return snake_game;
}

void SpawnApple(GameInfo_t *game_info) {
  int apple_x, apple_y;
  do {
    apple_x = GetPseudoRandomNumber() % FIELD_WIDTH + X_BEGIN;
    apple_y = GetPseudoRandomNumber() % FIELD_HEIGHT;
  } while (game_info->field[apple_y][apple_x] - '.');
  game_info->field[apple_y][apple_x] = 'O';
}

void ScheduledMove(time_t *initial_time) {
  SnakeGame *snake_game = SnakeGameInfo();
  time_t current_time;
  time(&current_time);
  if (difftime(current_time, *initial_time) >
      (TIMEOUTS[snake_game->game_info.level] / 5)) {
    SnakeMove();
    time(initial_time);
  }
}

void SnakeMove() {
  SnakeGame *snake_game = SnakeGameInfo();
  GameInfo_t *game_info = &snake_game->game_info;

  PlayerPos current_head = snake_game->snake_body.front();
  PlayerPos *new_head = &snake_game->new_head;
  PlayerPos current_tail = snake_game->snake_body.back();

  int motion_x = new_head->x - current_head.x;
  int motion_y = new_head->y - current_head.y;

  if (game_info->field[new_head->y][new_head->x] != '@') {
    if (game_info->field[new_head->y][new_head->x] == 'O') {
      game_info->score++;
      game_info->level = game_info->score / 5 % 10;
      SpawnApple(game_info);
    } else {
      snake_game->snake_body.pop_back();
    }
    snake_game->snake_body.push_front(snake_game->new_head);

    game_info->field[current_tail.y][current_tail.x] = '.';
    game_info->field[new_head->y][new_head->x] = '@';

    new_head->y += motion_y;
    if (new_head->y < 0) new_head->y = FIELD_HEIGHT;
    new_head->x += motion_x;

  } else {
    SnakeGameOver();
  }
}

void PrepareLeft() {
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos current_head = snake_game->snake_body.front();
  if (snake_game->new_head.x - current_head.x != 1) {
    snake_game->new_head.x = current_head.x - 1;
    snake_game->new_head.y = current_head.y;
  }
}

void PrepareRight() {
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos current_head = snake_game->snake_body.front();
  if (snake_game->new_head.x - current_head.x != -1) {
    snake_game->new_head.x = current_head.x + 1;
    snake_game->new_head.y = current_head.y;
  }
}

void PrepareUp() {
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos current_head = snake_game->snake_body.front();
  if (snake_game->new_head.y - current_head.y != 1) {
    snake_game->new_head.x = current_head.x;
    snake_game->new_head.y = current_head.y - 1;
    if (snake_game->new_head.y < 0) snake_game->new_head.y = FIELD_HEIGHT;
  }
}

void PrepareDown() {
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos current_head = snake_game->snake_body.front();
  if (snake_game->new_head.y - current_head.y != -1) {
    snake_game->new_head.x = current_head.x;
    snake_game->new_head.y = current_head.y + 1;
  }
}

void SnakeGameOver() {
  SnakeGame *snake_game = SnakeGameInfo();
  GameInfo_t *game_info = &snake_game->game_info;
  snake_game->state = GAME_OVER;

  FILE *high_score_file = fopen("logs/snake_high_score.txt", "r+");
  int high_score = 0;
  if (high_score_file) {
    fscanf(high_score_file, "%d", &high_score);
    if (game_info->score > high_score) {
      freopen("logs/snake_high_score.txt", "w", high_score_file);
      fprintf(high_score_file, "%d", game_info->score);
    }
  } else {
    high_score_file = fopen("logs/snake_high_score.txt", "w");
    fprintf(high_score_file, "%d", game_info->score);
  }
  fclose(high_score_file);
}
