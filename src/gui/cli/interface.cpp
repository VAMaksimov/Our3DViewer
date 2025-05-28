#include "include/interface.h"

#include "include/common.h"
#include "include/tetris.h"

UserAction_t TranslateUserInput(int ch) {
  UserAction_t action = NO_ACTION;
  switch (ch) {
    case ENTER_KEY:
      action = START;
      break;
    case ESCAPE:
      action = PAUSE;
      break;
    case KEY_LEFT:
      action = LEFT;
      break;
    case KEY_RIGHT:
      action = RIGHT;
      break;
    case KEY_UP:
      action = UP;
      break;
    case KEY_DOWN:
      action = DOWN;
      break;
    case SPACE_KEY:
      action = ACTION;
      break;
    default:
      break;
  }
  return action;
}

void Pause() {
  mvprintw(HINTS_Y, DISPLAY_INFO_X, "Press 'ESC' to unpause");
  mvprintw(HINTS_Y + 2, DISPLAY_INFO_X, "<PAUSED>");
  int ch = -1;
  while (ch != ESCAPE && ch != 'q') {
    ch = getch();
    usleep(100000);
  }
}

int CommonGameOver() {
  mvprintw(HINTS_Y, DISPLAY_INFO_X, "Game Over!           ");
  mvprintw(HINTS_Y + 1, DISPLAY_INFO_X, "Press 'ENTER' to start new game");
  mvprintw(HINTS_Y + 2, DISPLAY_INFO_X, "To quit press q");

  int ch = -1;
  while (ch != ENTER_KEY && ch != 'q') {
    ch = getch();
    usleep(100000);
  }
  return ch;
}

void PrintBoard(GameInfo_t *game_info) {
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; x++) {
      mvaddch(y, x, game_info->field[y][x]);
    }
  }

  PrintStats(game_info);

  refresh();
}

void PrintStats(GameInfo_t *game_info) {
  for (int y = HINTS_Y; y < HINTS_Y + 3; y++) {
    mvprintw(y, DISPLAY_INFO_X, "                                ");
  }
  for (int y = STATS_Y; y < STATS_Y + 3; y++) {
    mvprintw(y, DISPLAY_INFO_X, "                                ");
  }
  mvprintw(HINTS_Y, DISPLAY_INFO_X, "Press 'ESC' to pause");
  mvprintw(HINTS_Y + 1, DISPLAY_INFO_X, "Press 'ENTER' to start new game");
  mvprintw(STATS_Y, DISPLAY_INFO_X, "Score: %d", game_info->score);
  mvprintw(STATS_Y + 1, DISPLAY_INFO_X, "High score: %d",
           game_info->high_score);
  mvprintw(STATS_Y + 2, DISPLAY_INFO_X, "Level: %d", game_info->level);
}

void RenderTetrisPart() {
  mvprintw(NEXT_FIGURE_Y, DISPLAY_INFO_X, "Next figure: ");

  ShowFigure();
  DisplayNextFigure();
}

void ShowFigure() {
  TetrisGame tetris_game = *TetrisGameInfo();
  PlayerPos figure_pos = tetris_game.figure_pos;
  for (int pixel = 0; pixel < 4; pixel++) {
    int pixel_y =
        figure_pos.y + FIGURE_COORDS[tetris_game.current_figure][pixel][0];
    int pixel_x =
        figure_pos.x + FIGURE_COORDS[tetris_game.current_figure][pixel][1];
    mvaddch(pixel_y, pixel_x, '@');
  }
  int y = 0;
  while (tetris_game.game_info.field[y + 1][figure_pos.x] != '@') y++;
  mvaddch(y, figure_pos.x, 'O');
}

void DisplayNextFigure() {
  TetrisGame tetris_game = *TetrisGameInfo();
  for (int y = NEXT_FIGURE_Y + 4; y > NEXT_FIGURE_Y; y--) {
    for (int x = DISPLAY_INFO_X; x < DISPLAY_INFO_X + 4; x++) {
      mvaddch(y, x, ' ');
    }
  }
  for (int pixel = 0; pixel < 4; pixel++) {
    int pixel_y =
        NEXT_FIGURE_Y + 4 +
        FIGURE_COORDS[(tetris_game.current_figure + 1) % 28][pixel][0];
    int pixel_x =
        DISPLAY_INFO_X +
        FIGURE_COORDS[(tetris_game.current_figure + 1) % 28][pixel][1];
    mvaddch(pixel_y, pixel_x, '@');
  }
}
