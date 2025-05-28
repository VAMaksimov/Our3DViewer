#ifndef INCLUDE_INTERFACE_H
#define INCLUDE_INTERFACE_H

#include <ncurses.h>
#include <unistd.h>

#include <ctime>
#include <locale>

#include "include/common.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/**
 * Initialize ncurses mode
 *
 * @param time timeout for blocking input operations
 * @note The timeout value is in milliseconds
 */
#define InitializeNCurses(time)                                                \
  {                                                                            \
    setlocale(LC_ALL, ""); /** Set the locale to support UTF-8 characters */   \
    initscr();             /** Initialize the ncurses screen */                \
    noecho();              /** Disable echoing of typed characters */          \
    curs_set(0);           /** Hide the cursor from the screen */              \
    keypad(stdscr, TRUE);  /** Enable the keypad for capturing special keys */ \
    timeout(time); /** Set the timeout for blocking input operations */        \
  }

#define DISPLAY_INFO_X FIELD_WIDTH + 5
#define HINTS_Y 1
#define NEXT_FIGURE_Y HINTS_Y + 4
#define STATS_Y NEXT_FIGURE_Y + 6

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
UserAction_t TranslateUserInput(int ch);
void Pause();
int CommonGameOver();

void PrintBoard(GameInfo_t *game_info);
void PrintStats(GameInfo_t *game_info);

void RenderTetrisPart();
void ShowFigure();
void DisplayNextFigure();

#endif  // INCLUDE_INTERFACE_H