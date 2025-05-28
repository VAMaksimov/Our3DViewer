#include "include/common.h"

const float TIMEOUTS[10] = {2, 1.5, 1.25, 1, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3};

void InitialField(GameInfo_t *game_info) {
  for (int y = 0; y < FIELD_HEIGHT; y++) {
    game_info->field[y][0] = '@';
    game_info->field[y][FIELD_WIDTH + X_BEGIN] = '@';
    game_info->field[y][FIELD_WIDTH + X_BEGIN + 1] = '@';
    game_info->field[y][FIELD_WIDTH + X_BEGIN + 2] = '@';
    for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; x++) {
      game_info->field[y][x] = '.';
    }
  }
  for (int x = 0; x < FIELD_WIDTH + 4; x++) {
    game_info->field[FIELD_HEIGHT][x] = '@';
  }
}
