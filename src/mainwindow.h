#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include <QMainWindow>
#include <QVBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

    QHBoxLayout *mainLayout;
    QVBoxLayout *rightLayout;
    ChessBoard *chessBoard;

public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

};

#endif // MAINWINDOW_H
