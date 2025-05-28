#include "include/common.h"
#include "include/interface.h"
#include "include/snake.h"
#include "include/tetris.h"

/**
 * Logs to be saved in
 * logs/debug.log
 */
int main() {
  InitializeNCurses(INITIAL_TIMEOUT);
  printw("Do you wanna play snake or tetris\n");
  printw("Press 't' for tetris or 's' for snake\n");
  int ch = -1;
  while (ch != 't' && ch != 's') {
    ch = getch();
    usleep(100000);
  }
  clear();

  if (ch == 't') {
    TetrisGameLoop();
  } else if (ch == 's') {
    SnakeGameLoop();
  }
  endwin();
  return 0;
}
