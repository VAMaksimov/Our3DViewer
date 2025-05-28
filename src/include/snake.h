#ifndef INCLUDE_SNAKE_H
#define INCLUDE_SNAKE_H

#include <ctime>
#include <deque>

#include "include/common.h"

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define START_X FIELD_WIDTH / 2
#define START_Y FIELD_HEIGHT / 2

typedef void (*SnakeAction)();
extern const SnakeAction snake_table[9];

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct {
  GameInfo_t game_info;
  UserAction_t state;
  std::deque<PlayerPos> snake_body;
  PlayerPos new_head;
} SnakeGame;

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
SnakeGame *SnakeGameInfo();
void ResetSnake();

void SnakeGameLoop();
void SnakeUserInput(UserAction_t action);

int GetPseudoRandomNumber();
void SpawnApple(GameInfo_t *game_info);
void PrepareLeft();
void PrepareRight();
void PrepareUp();
void PrepareDown();
void SnakeGameOver();

SnakeGame InitSnakeGame();
void ScheduledMove(time_t *initial_time);
void SnakeMove();

#endif  // INCLUDE_SNAKE_H