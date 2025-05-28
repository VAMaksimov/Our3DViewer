#include "include/common.h"
#include "include/interface.h"
#include "include/snake.h"

const SnakeAction snake_table[9] = {ResetSnake,    Pause,     PrepareLeft,
                                    PrepareRight,  PrepareUp, PrepareDown,
                                    SnakeGameOver, SnakeMove, NULL};

void SnakeGameLoop() {
  SnakeGame *snake_game = SnakeGameInfo();
  UserAction_t *state = &snake_game->state;
  time_t initial_time;
  time(&initial_time);
  int ch = ENTER_KEY;
  do {
    *state = TranslateUserInput(ch);
    SnakeUserInput(*state);
    ScheduledMove(&initial_time);
    PrintBoard(&snake_game->game_info);
    if (*state != GAME_OVER) ch = getch();
    usleep(10000);  // 10 ms delay to prevent busy-waiting.
  } while (*state != GAME_OVER && ch != 'q');
  if (CommonGameOver() == ENTER_KEY) SnakeGameLoop();
}

void SnakeUserInput(UserAction_t action) {
  SnakeAction act = snake_table[action];
  if (act) act();
}
