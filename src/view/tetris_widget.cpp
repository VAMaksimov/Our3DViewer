#include "view/qt_widget.h"
#include "include/common.h"
#include "include/tetris.h"

#define NEXT_FIGURE_PADDING_X 2
#define NEXT_FIGURE_AREA_Y 4
namespace s21 {
TetrisWidget::TetrisWidget(QWidget *parent)
    : QWidget(parent), tetris_game(nullptr), game_timer(nullptr) {
  tetris_game = TetrisGameInfo();
  game_timer = new QTimer(this);

  setWindowTitle("Qt Tetris Game");
  int boardWidthPx = FIELD_WIDTH * CELL_SIZE;
  int boardHeightPx = FIELD_HEIGHT * CELL_SIZE;
  resize(boardWidthPx * 2, boardHeightPx + STATS_AREA_SIZE);

  connect(game_timer, &QTimer::timeout, this, &TetrisWidget::gameTick);

  setFocusPolicy(Qt::StrongFocus);

  StartGame();
}

TetrisWidget::~TetrisWidget() {
  delete game_timer;
  game_timer = nullptr;
  tetris_game = nullptr;
}

void TetrisWidget::keyPressEvent(QKeyEvent *event) {
  int key = event->key();

  UserAction_t new_state = tetris_game->state;
  new_state = NO_ACTION;

  switch (key) {
    case Qt::Key_Left:
      new_state = LEFT;
      MoveLeft();
      break;
    case Qt::Key_Right:
      new_state = RIGHT;
      MoveRight();
      break;
    case Qt::Key_Up:
      new_state = UP;
      Rotate();
      break;
    case Qt::Key_Down:
      new_state = DOWN;
      MoveDown();
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

void TetrisWidget::paintEvent(QPaintEvent *event) {
  Q_UNUSED(event);
  QPainter painter(this);

  GameInfo_t *game_info = &tetris_game->game_info;

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
  PrintInfo(painter, game_info, tetris_game->state);
  ShowFigure(painter);
  DisplayNextFigure(painter);
}

void TetrisWidget::PrintInfo(QPainter &painter, GameInfo_t *game_info,
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

void TetrisWidget::ShowFigure(QPainter &painter) {
  for (int block = 0; block < 4; ++block) {
    int relativeBlockY = FIGURE_COORDS[tetris_game->current_figure][block][0];
    int relativeBlockX = FIGURE_COORDS[tetris_game->current_figure][block][1];

    int x = relativeBlockX + tetris_game->figure_pos.x;
    int y = relativeBlockY + tetris_game->figure_pos.y;

    QRect cellRect(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE, CELL_SIZE);

    painter.fillRect(cellRect, Qt::darkGreen);
  }
}
void TetrisWidget::DisplayNextFigure(QPainter &painter) {
  int baseDisplayX =
      FIELD_WIDTH * CELL_SIZE + NEXT_FIGURE_PADDING_X * CELL_SIZE;

  int baseDisplayY = NEXT_FIGURE_AREA_Y * CELL_SIZE;

  int nextFigureIndex = (tetris_game->current_figure + 1) % 28;

  for (int block = 0; block < 4; ++block) {
    int relativeBlockY = FIGURE_COORDS[nextFigureIndex][block][0];
    int relativeBlockX = FIGURE_COORDS[nextFigureIndex][block][1];

    int absolutePixelX = baseDisplayX + relativeBlockX * CELL_SIZE;
    int absolutePixelY = baseDisplayY + relativeBlockY * CELL_SIZE;

    QRect cellRect(absolutePixelX, absolutePixelY, CELL_SIZE, CELL_SIZE);

    painter.fillRect(cellRect, Qt::darkGreen);
  }
}

void TetrisWidget::gameTick() {
  if (tetris_game->state != PAUSE && tetris_game->state != GAME_OVER) {
    MoveDown();

    if (tetris_game->state == GAME_OVER) {
      game_timer->stop();
    }
  }
  update();
  game_timer->setInterval(TIMEOUTS[tetris_game->game_info.level] * 1000);
}

void TetrisWidget::StartGame() {
  ResetTetris();
  game_timer->start(TIMEOUTS[tetris_game->game_info.level] * 1000);
  setFocus();
  update();
}

void TetrisWidget::TogglePause() {
  if (tetris_game->state == GAME_OVER) return;

  if (game_timer->isActive()) {
    game_timer->stop();
    tetris_game->state = PAUSE;
  } else {
    tetris_game->state = NO_ACTION;
    game_timer->start();
  }
  update();
}
}  // namespace s21
