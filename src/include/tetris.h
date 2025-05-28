#ifndef INCLUDE_TETRIS_H
#define INCLUDE_TETRIS_H

#include <ctime>

#include "include/common.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#define SPAWN_HEIGHT 3

#define FIGURE_AMOUNT 7

// extern is for include/interface.h
extern const int ARRAY_OF_POINTS[5];
extern const int FIGURE_COORDS[28][4][2];

typedef void (*TetrisAction)();
extern const TetrisAction tetris_table[9];

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct {
  GameInfo_t game_info;
  PlayerPos figure_pos;
  UserAction_t state;
  int current_figure;
} TetrisGame;

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
TetrisGame *TetrisGameInfo();
void ResetTetris();

void TetrisGameLoop();
void TetrisUserInput(UserAction_t action);

void SpawnFigure(GameInfo_t *game_info);
void MoveLeft();
void MoveRight();
void Rotate();
void MoveDown();
void TetrisGameOver();

TetrisGame InitTetrisGame();
void ScheduledFall(time_t *initial_time);
void MoveSideways(bool to_the_left);
int CollisionOccured();
void InsertFigureIntoField();
void TerminateRows();

#endif  // INCLUDE_TETRIS_H