#include "chessboard.h"
#include <QPainter>
#include <QBrush>
#include <QMouseEvent>

ChessBoard::ChessBoard(QWidget *parent) :
    QWidget(parent)
{
    setMinimumSize(500, 600);
    piece.clear();
}

void ChessBoard::changeStatus(ChessModel::ChessStatus status)
{
    piece = status;
    update();
}

void ChessBoard::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    if (width() < 400 || height() < 600) return;

    wid = (double)width() / 6;
    hei = (double)height() / 14;
    btnwid = wid * 0.3;
    btnhei = hei * 0.3;

    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);

    for (int i = 1; i <= 13; i++) {
        if (i == 7) continue;
        painter.drawLine(wid, i * hei, wid * 5, i * hei);
    }
    for (int i = 1; i <= 5; i++) {
        if (i == 2 || i == 4) {
            painter.drawLine(i * wid, hei, i * wid, hei * 6);
            painter.drawLine(i * wid, hei * 8, i * wid, hei * 13);
        } else {
            painter.drawLine(i * wid, hei, i * wid, hei * 13);
        }
    }
    painter.drawLine(wid, 2 * hei, 5 * wid, 6 * hei);
    painter.drawLine(wid, 4 * hei, 3 * wid, 6 * hei);
    painter.drawLine(3 * wid, 2 * hei, 5 * wid, 4 * hei);

    painter.drawLine(5 * wid, 2 * hei, wid, 6 * hei);
    painter.drawLine(3 * wid, 2 * hei, wid, 4 * hei);
    painter.drawLine(5 * wid, 4 * hei, 3 * wid, 6 * hei);

    painter.drawLine(wid, 8 * hei, 5 * wid, 12 * hei);
    painter.drawLine(wid, 10 * hei, 3 * wid, 12 * hei);
    painter.drawLine(3 * wid, 8 * hei, 5 * wid, 10 * hei);

    painter.drawLine(5 * wid, 8 * hei, wid, 12 * hei);
    painter.drawLine(3 * wid, 8 * hei, wid, 10 * hei);
    painter.drawLine(5 * wid, 10 * hei, 3 * wid, 12 * hei);

    QPen pen;
    pen.setWidth(4);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    painter.drawRect(wid, 2 * hei, 4 * wid, 10 * hei);
    painter.drawLine(wid, 6 * hei, 5 * wid, 6 * hei);
    painter.drawLine(wid, 8 * hei, 5 * wid, 8 * hei);
    painter.drawLine(3 * wid, 6 * hei, 3 * wid, 8 * hei);

    painter.setPen(QPen());

    QBrush brush(painter.brush());
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(Qt::white);
    painter.setBrush(brush);
    for (int i = 1; i <= 13; i++)
        for (int j = 1; j <= 5; j++) {
            if (i == 7 && j % 2 == 0) {
                painter.setBrush(QBrush());
                double cc = std::min(std::min(hei, wid) - 5, std::max(btnwid, btnhei));
                painter.drawEllipse(j * wid - cc, i * hei - cc, cc * 2, cc * 2);
                cc -= 3;
                painter.drawEllipse(j * wid - cc, i * hei - cc, cc * 2, cc * 2);

                painter.setBrush(brush);
                continue;
            }
            if ((i == 1 || i == 13) && j % 2 == 0) {
                brush.setColor(Qt::black);
                painter.setBrush(brush);
                painter.drawRect(j * wid - 1.5 * btnwid, i * hei - btnhei, 3 * btnwid, 2 * btnhei);
                painter.drawEllipse(j * wid - btnwid, i * hei - ((i == 13) ? 2 * btnhei : 0), 2 * btnwid, 2 * btnhei);
                brush.setColor(Qt::white);
                painter.setBrush(brush);
                continue;
            }
            if ((i % 2 == 1 && j % 2 == 0) || (j == 3 && (i == 4 || i == 10))) {
                painter.drawEllipse(j * wid - btnwid * 1.3, i * hei - btnhei * 1.3, 2.6 * btnwid, 2.6 * btnhei);
                continue;
            }
            painter.drawRect(j * wid - btnwid, i * hei - btnhei, 2 * btnwid, 2 * btnhei);
        }

    for (auto itm : piece) {
        int i = itm.first.first, j = itm.first.second;
        if (itm.second > 0) {
            brush.setColor(Qt::green);
            painter.setBrush(brush);
            painter.drawRect(j * wid - btnwid * 1.2, i * hei - btnhei * 1.2, 2.4 * btnwid, 2.4 * btnhei);
            painter.drawText(j * wid - btnwid * 1.2, i * hei - btnhei * 1.2, 2.4 * btnwid, 2.4 * btnhei,
                             Qt::AlignCenter, ChessModel::pieceName[itm.second]);
        }
        if (itm.second < 0) {
            brush.setColor(Qt::red);
            painter.setBrush(brush);
            painter.drawRect(j * wid - btnwid * 1.2, i * hei - btnhei * 1.2, 2.4 * btnwid, 2.4 * btnhei);
            painter.drawText(j * wid - btnwid * 1.2, i * hei - btnhei * 1.2, 2.4 * btnwid, 2.4 * btnhei,
                             Qt::AlignCenter, ChessModel::pieceName[-itm.second]);
        }
    }
}

void ChessBoard::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    for (int i = 1; i <= 13; i++)
        for (int j = 1; j <= 5; j++)
            if (QRect(j * wid - btnwid * 1.2, i * hei - btnhei * 1.2, 2.4 * btnwid, 2.4 * btnhei).contains(event->pos()))
                emit clickPiece(qMakePair(i, j));
}
