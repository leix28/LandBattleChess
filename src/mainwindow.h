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
#include <QLabel>
#include <QTimer>
#include <QLCDNumber>
#include "user.h"
#include "userwidget.h"
#include "myinputdialog.h"

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
    bool isStart, isWaiting;
    User locUser, oppUser;
    char *u, *i;
    int lenu, leni, pu, pi;
    UserWidget *opp, *loc;
    QLabel *oppName, *locName;
    QPushButton *startButton, *tieButton, *surButton;
    QLCDNumber *number;
    QTimer tieTimer, timer;
    int res;

    void serverSend(QByteArray a, char pre);
    void clientSend(QByteArray a, char pre);


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
    void startGame();
    void enableTie();
    void handleTie();
    void timeOut();
};

#endif // MAINWINDOW_H
