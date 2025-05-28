#include "include/common.h"
#include "include/interface.h"
#include "include/tetris.h"

const TetrisAction tetris_table[9] = {ResetTetris,    Pause,  MoveLeft,
                                      MoveRight,      Rotate, MoveDown,
                                      TetrisGameOver, NULL,   NULL};

void TetrisGameLoop() {
  TetrisGame *tetris_game = TetrisGameInfo();
  UserAction_t *state = &tetris_game->state;
  time_t initial_time;
  time(&initial_time);
  int ch = ENTER_KEY;
  do {
    *state = TranslateUserInput(ch);
    TetrisUserInput(*state);
    PrintBoard(&tetris_game->game_info);
    RenderTetrisPart();
    ScheduledFall(&initial_time);
    if (*state != GAME_OVER) ch = getch();
    usleep(10000);  // 10 ms delay to prevent busy-waiting.
  } while (*state != GAME_OVER && ch != 'q');
  if (CommonGameOver() == ENTER_KEY) TetrisGameLoop();
}

void TetrisUserInput(UserAction_t action) {
  TetrisAction act = tetris_table[action];
  if (act) act();
}
