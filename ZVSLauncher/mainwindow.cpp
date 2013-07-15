#include "mainwindow.h"
#include <QPainter>
#include <QColor>
#include <QLinearGradient>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    m_cache(NULL)
{
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    resize(800, 600);

    movingWindows = false;
}


void MainWindow::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);

    m_cache = new QPixmap(size());
    m_cache->fill(Qt::transparent);
    QPainter painter(m_cache);

    // Bakground
    QPolygon bkg;
    QColor bkgColor;
    bkgColor.setRgb(80, 80, 80, 200);

    bkg << QPoint(0, 40)
          << QPoint(20, 20)
          << QPoint((width() / 2) - (width() / 4) - 20, 20)
          << QPoint((width() / 2) - (width() / 4),  0)
          << QPoint((width() / 2) + (width() / 4),  0)
          << QPoint((width() / 2) + (width() / 4) + 20, 20)
          << QPoint(width() - 20, 20)
          << QPoint(width(), 40)
          << QPoint(width(), height() - 20)
          << QPoint(width() - 20, height())
          << QPoint(20, height())
          << QPoint(0, height() - 20);

    painter.setPen(Qt::SolidLine);
    painter.setBrush(QBrush(bkgColor));

    painter.drawPolygon(bkg);

    //Border

    QPolygon border, smallBorder;
    QColor colorBorder1;
    colorBorder1.setRgb(100, 140, 60, 255);
    QColor colorBorder2;
    colorBorder2.setRgb(100, 140, 60, 255);
    QLinearGradient linearGradBorder(QPointF(100, 100), QPointF(300, 400));
    linearGradBorder.setColorAt(0, colorBorder1);
    linearGradBorder.setColorAt(1, colorBorder2);

    //point
    QPoint e1(20, 20),
            e2((width() / 2) - (width() / 4) - 20, 20),
            e3((width() / 2) - (width() / 4),  0),
            e4((width() / 2) + (width() / 4),  0),
            e5((width() / 2) + (width() / 4) + 20, 20), // point e(x) sont a l'exterieur les i(x) sont les point interieur
            e6(width() - 20, 20),                       //on part du point en haut a droite au dessus du biseau
            e7(width(), 40),
            e8(width(), height() - 20),
            e9(width() - 20, height()),
            e10(20, height()),
            e11(0, height() - 20),
            e12(0, 40);

    border << e1
         << e2
         << e3
         << e4
         << e5
         << e6
         << e7
         << e8
         << e9
         << e10
         << e11
         << e12
         << e1;

    smallBorder << e2
                << QPoint((width() / 2) - (width() / 4), 40)
                << QPoint((width() / 2) + (width() / 4), 40)
                << e5;

    // Zone Buttons
    QPolygon buttonZone;
    QColor buttonZoneColor;
    buttonZoneColor.setRgb(250, 80, 80);

    buttonZone << QPoint(width() - 110, 30)
        << QPoint(width() - 30, 30)
        << QPoint(width() - 10, 50)
        << QPoint(width() - 90, 50)
             ;

    painter.setPen(Qt::SolidLine);
    painter.setBrush(QBrush(buttonZoneColor));

    painter.drawPolygon(buttonZone);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setWidth(6);
    pen.setBrush(QBrush(linearGradBorder));
    painter.setPen(pen);
    painter.scale(0.995, 0.995);
    painter.translate(2, 2);

    painter.drawPolyline(border);
    painter.drawPolyline(smallBorder);

    pen.setBrush(QBrush(Qt::white));
    painter.setPen(pen);
    QFont font;
    font.setStyleHint(QFont::Cursive);
    painter.setFont(font);
    painter.drawText((width() / 2) - 50, 5, 70, 25, Qt::AlignCenter, "ZVS Launcher");
}

void MainWindow::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);

  if(m_cache != NULL)
  {
    QPainter painter(this);

    painter.drawPixmap(0, 0, *m_cache);
  }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(isInsideBar(event->pos()))
    {
        setCursor(QCursor(Qt::SizeAllCursor));
        movingWindows = true;
        m_Diff = event->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    movingWindows = false;
    setCursor(QCursor(Qt::ArrowCursor));
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(!movingWindows)
        return;

    QPoint p = event->globalPos();

    move(p - m_Diff);
}

bool MainWindow::isInsideBar(QPoint const &point)
{
    if(point.x() > (width() / 2) - (width() / 4)
            && point.x() < (width() / 2) + (width() / 4)
            && point.y() > 0
            && point.y() < 40)
        return true;

    return false;
}

