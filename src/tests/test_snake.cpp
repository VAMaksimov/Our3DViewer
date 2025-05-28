#include "gtest/gtest.h"
#include "include/common.h"
#include "include/snake.h"

TEST(SnakeGameTest, SnakeGameInfo) {
  SnakeGame *snake_game = SnakeGameInfo();
  ASSERT_NE(snake_game, nullptr);
  EXPECT_EQ(snake_game->state, START);
  EXPECT_EQ(snake_game->snake_body.size(), 4);
  EXPECT_EQ(snake_game->game_info.score, 0);
}

TEST(SnakeGameTest, ResetSnake) {
  SnakeGame *snake_game = SnakeGameInfo();
  snake_game->state = GAME_OVER;
  snake_game->snake_body.clear();
  snake_game->game_info.score = 100;
  ResetSnake();
  EXPECT_EQ(snake_game->state, START);
  EXPECT_EQ(snake_game->snake_body.front().y, FIELD_HEIGHT / 2);
  EXPECT_EQ(snake_game->snake_body.size(), 4);
  EXPECT_EQ(snake_game->game_info.score, 0);
}

TEST(SnakeGameTest, GetPseudoRandomNumberRange) {
  int random_number = GetPseudoRandomNumber();
  EXPECT_GE(random_number, 0);
  EXPECT_LT(random_number, FIELD_HEIGHT * FIELD_WIDTH);
}

TEST(SnakeGameTest, InitSnakeGameState) {
  SnakeGame snake_game = InitSnakeGame();
  EXPECT_EQ(snake_game.state, START);
  EXPECT_EQ(snake_game.snake_body.size(), 4);
  EXPECT_EQ(snake_game.new_head.x, START_X);
  EXPECT_EQ(snake_game.new_head.y, START_Y + 1);
  EXPECT_EQ(snake_game.game_info.score, 0);
  EXPECT_EQ(snake_game.game_info.level, 0);
}

TEST(SnakeGameTest, SpawnApple) {
  SnakeGame *snake_game = SnakeGameInfo();
  SpawnApple(&snake_game->game_info);
  bool apple_found = false;
  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; ++x) {
      if (snake_game->game_info.field[y][x] == 'O') {
        apple_found = true;
        break;
      }
    }
    if (apple_found) break;
  }
  EXPECT_TRUE(apple_found);
}

TEST(SnakeGameTest, SnakeMove) {
  fopen("logs/snake_high_score.txt", "w");
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos initial_head = snake_game->snake_body.front();
  SnakeMove();
  PlayerPos new_head = snake_game->snake_body.front();
  EXPECT_EQ(new_head.x, initial_head.x);
  EXPECT_EQ(new_head.y, initial_head.y + 1);
  snake_game->game_info.field[initial_head.y + 2][initial_head.x] = 'O';
  SnakeMove();
  EXPECT_EQ(snake_game->snake_body.size(), 5);
}

TEST(SnakeGameTest, SnakeGameOver) {
  SnakeGame *snake_game = SnakeGameInfo();
  SnakeGameOver();
  EXPECT_EQ(snake_game->state, GAME_OVER);
  FILE *high_score_file = fopen("logs/snake_high_score.txt", "r");
  int high_score = 0;
  if (high_score_file) {
    fscanf(high_score_file, "%d", &high_score);
    fclose(high_score_file);
  }
  EXPECT_EQ(high_score, 1);
}

TEST(SnakeGameTest, Turns) {
  SnakeGame *snake_game = SnakeGameInfo();
  PlayerPos initial_head = snake_game->snake_body.front();
  PrepareLeft();
  EXPECT_EQ(snake_game->new_head.x, initial_head.x - 1);
  EXPECT_EQ(snake_game->new_head.y, initial_head.y);
  PrepareUp();
  EXPECT_EQ(snake_game->new_head.x, initial_head.x);
  EXPECT_EQ(snake_game->new_head.y, initial_head.y - 1);
  PrepareRight();
  EXPECT_EQ(snake_game->new_head.x, initial_head.x + 1);
  EXPECT_EQ(snake_game->new_head.y, initial_head.y);
  PrepareDown();
  EXPECT_EQ(snake_game->new_head.x, initial_head.x);
  EXPECT_EQ(snake_game->new_head.y, initial_head.y + 1);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
