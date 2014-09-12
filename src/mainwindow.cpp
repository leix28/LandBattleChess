#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    server(500, this),
    client(500, this)
{
    setMenuBar(new QMenuBar(this));
    QMenu *gameMenu = new QMenu("Game", menuBar());
    createAction = new QAction("Create", gameMenu);
    createAction->setIcon(QIcon(":/create.icns"));
    joinAction = new QAction("Join in", gameMenu);
    joinAction->setIcon(QIcon(":/join.icns"));
    abortAction = new QAction("Abort", gameMenu);
    abortAction->setIcon(QIcon(":/abort.icns"));
    menuBar()->addMenu(gameMenu);
    gameMenu->addAction(createAction);
    gameMenu->addAction(joinAction);
    gameMenu->addAction(abortAction);
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(0);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->addAction(createAction);
    toolbar->addAction(joinAction);
    toolbar->addAction(abortAction);
    abortAction->setEnabled(0);

    setStatusBar(new QStatusBar(this));


    setCentralWidget(new QWidget(this));
    QHBoxLayout *mainLayout = new QHBoxLayout();
    centralWidget()->setLayout(mainLayout);

    chessBoard = new ChessBoard(centralWidget());
    mainLayout->addWidget(chessBoard);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout);

    //TODO rightLayout for some information


    QObject::connect(createAction, SIGNAL(triggered()), this, SLOT(createGame()));
    QObject::connect(joinAction, SIGNAL(triggered()), this, SLOT(joinGame()));
    QObject::connect(abortAction, SIGNAL(triggered()), this, SLOT(abortLink()));
    QObject::connect(chessBoard, SIGNAL(clickPiece(QPair<int,int>)), this, SLOT(handleClick(QPair<int, int>)));
    QObject::connect(&server, SIGNAL(received(void*, int)), this, SLOT(handleReceive(void*, int)));
    QObject::connect(&server, SIGNAL(connected()), this, SLOT(handleConnected()));
    QObject::connect(&client, SIGNAL(received(void*,int)), this, SLOT(handleReceive(void*, int)));
}

void MainWindow::createGame()
{
    isStart = 0;
    lastPos = qMakePair(0, 0);
    if (server.closeServer() &&
            client.closeClient() &&
            server.initServer((char*)"127.0.0.1", 4321) &&
            server.startServer()) {
        statusBar()->showMessage("Server Created.");
        createAction->setEnabled(0);
        joinAction->setEnabled(0);
        abortAction->setEnabled(1);

    } else
        statusBar()->showMessage("Fail: create");
    player = 'A';
}

void MainWindow::joinGame()
{
    isStart = 0;
    lastPos = qMakePair(0, 0);
    if (server.closeServer() &&
            client.closeClient() &&
            client.connectServer((char*)"127.0.0.1", 4321)) {
        statusBar()->showMessage("Server connected.");
        createAction->setEnabled(0);
        joinAction->setEnabled(0);
        abortAction->setEnabled(1);
    } else
        statusBar()->showMessage("Fail: connect.");
    player = 'B';

}

void MainWindow::abortLink()
{
    server.closeServer();
    client.closeClient();
    chessBoard->changeStatus(ChessModel::ChessStatus());
    createAction->setEnabled(1);
    joinAction->setEnabled(1);
    abortAction->setEnabled(0);
}

void MainWindow::handleClick(QPair<int, int> pos)
{
    if (isStart) {
        if (lastPos.first && lastPos.second) {
            if (chessModel.isMovable(player, lastPos, pos)) {
                statusBar()->showMessage("Waiting");
                chessModel.movePiece(player, lastPos, pos);
                chessBoard->changeStatus(chessModel.getStatus(player));
                char *buf = new char[sizeof(ChessModel) + 1];
                buf[0] = 0;
                memcpy(buf + 1, &chessModel, sizeof(ChessModel));
                if (player == 'A')
                    server.sendData(buf, sizeof(ChessModel) + 1);
                else
                    client.sendData(buf, sizeof(ChessModel) + 1);
                delete buf;
                lastPos = qMakePair(0, 0);
                return;
            } else {
                if (chessModel.isOwner(player, pos)) {
                    lastPos = pos;
                    statusBar()->showMessage(QString("Select (%1, %2)").arg(pos.first).arg(pos.second));
                } else {
                    lastPos = qMakePair(0, 0);
                    statusBar()->showMessage(QString("Your Turn"));
                }
            }
        } else {
            if (chessModel.isOwner(player, pos)) {
                lastPos = pos;
                statusBar()->showMessage(QString("Select (%1, %2)").arg(pos.first).arg(pos.second));
            } else
                statusBar()->showMessage(QString("Your Turn"));
        }
    } else {
    }
}

void MainWindow::handleReceive(void *bufv, int len)
{
    qDebug() << "receive";
    char *buf = (char*)bufv;
    if (buf[0] == 0 && len == sizeof(ChessModel) + 1) {
        if (isStart)
            statusBar()->showMessage(QString("Your Trun"));
        else
            statusBar()->showMessage(QString("Please Organize Your Chess"));
        chessModel = *(ChessModel*)(buf + 1);
        chessBoard->changeStatus(chessModel.getStatus(player));
        delete buf;
    }
}

void MainWindow::handleConnected()
{
    chessModel.initStatus();
    chessBoard->changeStatus(chessModel.getStatus(player));

    char *buf = new char[sizeof(ChessModel) + 1];
    buf[0] = 0;
    memcpy(buf + 1, &chessModel, sizeof(ChessModel));
    server.sendData(buf, sizeof(ChessModel) + 1);
    delete buf;
    statusBar()->showMessage(QString("Please Organize Your Chess"));
}

