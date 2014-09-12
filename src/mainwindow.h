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
    QAction *createAction, *joinAction, *abortAction;
    EzServer server;
    EzClient client;
    char player;
    QPair<int, int> lastPos;
    bool isStart;

public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:
    void createGame();
    void joinGame();
    void abortLink();
    void handleClick(QPair<int, int> pos);
    void handleReceive(void* bufv, int len);
    void handleConnected();
};

#endif // MAINWINDOW_H
