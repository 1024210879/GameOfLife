#include "GameOfLife.h"
#include "ui_GameOfLife.h"

GameOfLife::GameOfLife(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOfLife)
{
    ui->setupUi(this);

    this->init();
}

GameOfLife::~GameOfLife()
{
    delete ui;
}

void GameOfLife::paintEvent(QPaintEvent *event)
{
    static const QBrush bAlive = QBrush(QColor(10, 120, 20, 140));
    static const QBrush bDead  = QBrush(QColor(30, 30, 30, 50));

    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < g_cellny; ++i)
        for (int j = 0; j < g_cellnx; ++j)
        {
            painter.setBrush(ALIVE == m_cells[i][j].cur ? bAlive : bDead);
            painter.drawRect(j*(g_cellw + 2)+1, i*(g_cellh + 2)+1, g_cellw, g_cellh);
        }
}

void GameOfLife::init()
{
    this->setFixedSize(g_canvasw, g_canvash);

    std::srand(time(0));
    for (int i = 0; i < g_cellny; ++i)
        for (int j = 0; j < g_cellnx; ++j)
            m_cells[i][j] = CellStatus{ rand()%3==0 ? ALIVE : DEAD, DEAD };

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCells()));
    m_timer.start(g_interval);
}

void GameOfLife::updateCells()
{
    int row, col, _row, _col, cnt, i, j;
    for (row = 0; row < g_cellny; ++row)
    {
        for (col = 0; col < g_cellnx; ++col)
        {
            // 周围存活数量（包括自己）
            cnt = 0;
            for (i = 0; i < 3; ++i)
            {
                _row = (row + g_offset[i] + g_cellny) % g_cellny;
                for (j = 0; j < 3; ++j)
                {
                    _col = (col + g_offset[j] + g_cellnx) % g_cellnx;
                    if (ALIVE == m_cells[_row][_col].cur)
                    {
                        ++cnt;
                    }
                }
            }
            --cnt;

            // 自己存活/死亡时
            if (ALIVE == m_cells[row][col].cur)
            {
                m_cells[row][col].next = (cnt < 2 || cnt > 3) ? DEAD : ALIVE;
            }
            else
            {
                m_cells[row][col].next = (cnt == 2 || cnt == 3) ? ALIVE : DEAD;
            }
        }
    }

    for (row = 0; row < g_cellny; ++row)
        for (col = 0; col < g_cellnx; ++col)
            m_cells[row][col].cur = m_cells[row][col].next;

    this->update();
}
