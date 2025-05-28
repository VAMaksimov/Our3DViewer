#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#include <cstdio>

// CONSTANTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define FIELD_HEIGHT 20
#define FIELD_WIDTH 10

#define X_BEGIN 1

#define ESCAPE 27
#define ENTER_KEY 10
#define SPACE_KEY 32

#define INITIAL_TIMEOUT 150

#define SUCCESS 0
#define ERROR 1

// extern is for include/interface.h
extern const float TIMEOUTS[10];

// STRUCTS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
typedef struct {
  int x;
  int y;
} PlayerPos;

typedef enum {
  START,
  PAUSE,
  LEFT,
  RIGHT,
  UP,
  DOWN,
  GAME_OVER,
  ACTION,
  NO_ACTION
} UserAction_t;

typedef struct {
  int field[FIELD_HEIGHT + 1][FIELD_WIDTH + 4];
  int score;
  int high_score;
  int level;
} GameInfo_t;

// FUNCTIONS >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void InitialField(GameInfo_t *game_info);

#endif  // INCLUDE_COMMON_H