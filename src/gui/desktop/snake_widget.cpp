#include "gui/desktop/qt_widget.h"
#include "include/common.h"
#include "include/snake.h"

namespace s21 {
/**
 * @brief The SnakeWidget::SnakeWidget constructor initializes a SnakeWidget
 * object, inheriting from QWidget. It sets up the game environment by
 * initializing a SnakeGameInfo instance, creating a QTimer for game ticks,
 * configuring the window properties, connecting the timer to the gameTick
 * method, and starting the game with StartGame().
 */
SnakeWidget::SnakeWidget(QWidget *parent)
    : QWidget(parent), snake_game(nullptr), game_timer(nullptr) {
  snake_game = SnakeGameInfo();
  game_timer = new QTimer(this);

  setWindowTitle("Qt Snake Game");
  int boardWidthPx = FIELD_WIDTH * CELL_SIZE;
  int boardHeightPx = FIELD_HEIGHT * CELL_SIZE;
  resize(boardWidthPx * 2, boardHeightPx + STATS_AREA_SIZE);

  connect(game_timer, &QTimer::timeout, this, &SnakeWidget::gameTick);

  setFocusPolicy(Qt::StrongFocus);

  StartGame();
}

SnakeWidget::~SnakeWidget() {
  delete game_timer;
  game_timer = nullptr;
  snake_game = nullptr;
}

/**
 * @brief The SnakeWidget::keyPressEvent method handles keyboard input for the
 * SnakeWidget class by overriding the base class implementation. It processes
 * various key events to control the game state, such as moving the snake,
 * starting or restarting the game, toggling pause, or quitting, while passing
 * unhandled keys to the base class.
 */
void SnakeWidget::keyPressEvent(QKeyEvent *event) {
  int key = event->key();

  UserAction_t new_state = snake_game->state;
  new_state = NO_ACTION;

  switch (key) {
    case Qt::Key_Left:
      new_state = LEFT;
      PrepareLeft();
      break;
    case Qt::Key_Right:
      new_state = RIGHT;
      PrepareRight();
      break;
    case Qt::Key_Up:
      new_state = UP;
      PrepareUp();
      break;
    case Qt::Key_Down:
      new_state = DOWN;
      PrepareDown();
      break;
    case Qt::Key_Space:
      new_state = ACTION;
      SnakeMove();
      break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
      new_state = START;
      StartGame();
      break;
    case Qt::Key_Escape:
      TogglePause();
      break;
    case Qt::Key_Q:
      qApp->quit();
      break;
    default:
      QWidget::keyPressEvent(event);
      break;
  }
  update();
}

/**
 * @brief The PaintEvent method is responsible for rendering the
 * game board and its elements. It is called whenever the widget needs to be
 * repainted. The method uses a QPainter object to draw the game field,
 * including the snake, food, and game status messages. The method iterates
 * over the game field array and fills each cell with the appropriate color
 * based on the character representing the snake or food. It also calls
 * PrintInfo() to display the game statistics and status messages.
 *
 * ---
 *
 * @note The Q_UNUSED macro is used to explicitly indicate that the parameter
 * 'event' is intentionally unused in this function. This prevents compiler
 * warnings about unused parameters, which can occur when a parameter is
 * declared but not referenced in the function body. In this case, the
 * QPaintEvent parameter 'event' is not needed because the painting logic
 * does not rely on the specific details of the event.
 *
 * Using Q_UNUSED is a common practice in Qt to maintain clean and
 * warning-free code, especially in overridden methods where not all
 * parameters are always required.
 *
 * ---
 *
 * The QPainter object 'painter' is instantiated with 'this' as its
 * parent widget. QPainter is a Qt class used for custom rendering and drawing
 * operations on widgets. By associating it with 'this', the current widget
 * (SnakeWidget) becomes the target for all drawing operations performed by
 * the painter. This allows the game board and other visual elements to be
 * rendered directly onto the widget.
 */
void SnakeWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  GameInfo_t *game_info = &snake_game->game_info;

  for (int y = 0; y < FIELD_HEIGHT; ++y) {
    for (int x = X_BEGIN; x < FIELD_WIDTH + X_BEGIN; ++x) {
      char tile = game_info->field[y][x];
      QRect cellRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);

      QColor cellColor = Qt::black;
      switch (tile) {
        case '@':
          cellColor = Qt::darkGreen;
          break;
        case 'O':
          cellColor = Qt::red;
          break;
      }
      painter.fillRect(cellRect, cellColor);
    }
  }
  PrintInfo(painter, game_info, snake_game->state);
}

void SnakeWidget::PrintInfo(QPainter &painter, GameInfo_t *game_info,
                            UserAction_t state) {
  int boardHeightPx = FIELD_HEIGHT * CELL_SIZE;
  QRect statsRect(0, boardHeightPx, width(), STATS_AREA_SIZE);
  painter.fillRect(statsRect, Qt::lightGray);
  painter.setPen(Qt::black);

  int textPadding = 5;
  int lineHeight = QFontMetrics(painter.font()).height();
  int currentY = boardHeightPx + textPadding;

  painter.drawText(textPadding, currentY + lineHeight,
                   QString("Score: %1").arg(game_info->score));
  painter.drawText(textPadding, currentY + 2 * lineHeight,
                   QString("High Score: %1").arg(game_info->high_score));
  painter.drawText(textPadding, currentY + 3 * lineHeight,
                   QString("Level: %1").arg(game_info->level));

  int hintsX = width() * 3 / 10;
  painter.drawText(hintsX, currentY + lineHeight, "Press 'ESC' to pause");
  painter.drawText(hintsX, currentY + 2 * lineHeight,
                   "Press 'ENTER' to start new game");
  painter.drawText(hintsX, currentY + 3 * lineHeight, "Press 'Q' to quit");

  if (state == GAME_OVER) {
    painter.setPen(Qt::red);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter,
                     "GAME OVER\nPress Enter to Restart");
  } else if (state == PAUSE) {
    painter.setPen(Qt::blue);
    painter.setFont(QFont("Arial", 16, QFont::Bold));
    painter.drawText(rect(), Qt::AlignCenter, "PAUSED\nPress Esc to Resume");
  }
}

/**
 * @brief The gameTick method is responsible for updating the game
 * state at regular intervals. It checks the current game state and performs
 * actions accordingly, such as moving the snake or handling game over
 * conditions. The method is connected to a QTimer, which triggers it at
 * specified intervals to create the game loop.
 */
void SnakeWidget::gameTick() {
  if (snake_game->state != PAUSE && snake_game->state != GAME_OVER) {
    SnakeMove();

    if (snake_game->state == GAME_OVER) {
      game_timer->stop();
    }
  }
  update();
  game_timer->setInterval(TIMEOUTS[snake_game->game_info.level] * 250);
}

/**
 * @brief The SnakeWidget::StartGame method initializes the game by resetting
 * the snake's position, starting the game timer, and ensuring that the widget
 * is ready to receive key presses by calling setFocus(). It also triggers an
 * initial update to draw the game board.
 */
void SnakeWidget::StartGame() {
  ResetSnake();
  game_timer->start(TIMEOUTS[snake_game->game_info.level] * 250);
  setFocus();
  update();
}

void SnakeWidget::TogglePause() {
  if (snake_game->state == GAME_OVER) return;

  if (game_timer->isActive()) {
    game_timer->stop();
    snake_game->state = PAUSE;
  } else {
    snake_game->state = NO_ACTION;
    game_timer->start();
  }
  update();
}
}  // namespace s21
