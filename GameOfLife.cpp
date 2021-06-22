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
    static const QBrush bAlive = QBrush(QColor(10, 120, 20, 255));
    static const QBrush bDead  = QBrush(QColor(0, 0, 0, 255));

    QPainter painter(&m_image);
    painter.setPen(Qt::NoPen);

    for (int i = 0; i < g_cellny; ++i)
        for (int j = 0; j < g_cellnx; ++j)
        {
            painter.setBrush(ALIVE == m_cells[i][j].cur ? bAlive : bDead);
            painter.drawRect(j*g_cellw, i*g_cellh, g_cellw, g_cellh);
        }

    static int counter = 0;
    m_image.save(QString("%1/%2.bmp").arg(g_strSave).arg(QString::number(++counter)));

    QPainter painter2(this);
    painter2.drawImage(this->rect(), m_image);
}

void GameOfLife::init()
{
    createDir();

    this->setFixedSize(g_canvasw, g_canvash);

    m_image = QImage(g_canvasw, g_canvash, QImage::Format_RGB888);

    std::srand(time(0));
    for (int i = g_cellny/2-5; i < g_cellny/2+0; ++i)
        for (int j = g_cellnx/2-5; j < g_cellnx/2+5; ++j)
            m_cells[i][j] = CellStatus{ rand()%g_randSeed==0 ? ALIVE : DEAD, DEAD };

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(updateCells()));
    m_timer.start(g_interval);
}

void GameOfLife::createDir()
{
    if (!QDir().exists(g_strSave))
    {
        QDir().mkdir(g_strSave);
    }
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
