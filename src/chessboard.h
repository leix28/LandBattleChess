#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include "chessmodel.h"
#include <QWidget>
#include <QVector>

class ChessBoard : public QWidget
{
    Q_OBJECT

    QVector< QPair< QPair<int, int>, int> > piece;

public:
    explicit ChessBoard(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:

};

#endif // CHESSBOARD_H
