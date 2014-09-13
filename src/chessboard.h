#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "chessmodel.h"
#include <QWidget>
#include <QVector>

class ChessBoard : public QWidget
{
    Q_OBJECT

    ChessModel::ChessStatus piece;
    double wid, hei, btnwid, btnhei;

public:
    explicit ChessBoard(QWidget *parent = 0);
    void changeStatus(ChessModel::ChessStatus status);

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);

signals:
    void clickPiece(QPair<int, int> pos);
    void rightClickPiece(QPair<int, int> pos);
public slots:

};

#endif // CHESSBOARD_H
