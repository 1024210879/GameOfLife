#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QWidget>
#include <QTimer>
#include <QPainter>
#include <QDir>
#include <time.h>
#include <iostream>
#include <QImage>

static const int g_randSeed = 10;
static const int g_interval = 150;
static const int g_canvasw = 600;
static const int g_canvash = 600;
static const int g_cellnx = 300;
static const int g_cellny = 300;
static const int g_celln = g_cellnx * g_cellny;
static const int g_cellw = g_canvasw / g_cellnx;
static const int g_cellh = g_canvash / g_cellny;
static const int g_offset[3] = { -1, 0, 1 };

static QString g_strSave = "./output";

enum IsAlive {
    DEAD  = 0,
    ALIVE = 1
};

namespace Ui {
class GameOfLife;
}

class GameOfLife : public QWidget
{
    Q_OBJECT

public:
    explicit GameOfLife(QWidget *parent = 0);
    ~GameOfLife();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::GameOfLife *ui;
    QTimer m_timer;
    QImage m_image;

    typedef struct {
        IsAlive cur;
        IsAlive next;
    }CellStatus;
    CellStatus m_cells[g_cellny][g_cellnx];

private:
    void init();
    void createDir();

private slots:
    void updateCells();
};

#endif // GAMEOFLIFE_H
