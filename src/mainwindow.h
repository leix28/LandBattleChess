#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include "chessmodel.h"
#include "ezsocket.h"
#include <QMainWindow>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ChessBoard *chessBoard;
    ChessModel chessModel;
    EzServer server;
    EzClient client;
    char player;

public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void createGame();
    void joinGame();
    void handleClick(QPair<int, int> pos);
    void handleReceive(void* bufv, int len);

};

#endif // MAINWINDOW_H
