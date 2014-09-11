#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    server(400, this),
    client(400, this)
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

    chessBoard->changeStatus(chessModel.getStatus('B'));

    QObject::connect(createAction, SIGNAL(triggered()), this, SLOT(createGame()));
    QObject::connect(joinAction, SIGNAL(triggered()), this, SLOT(joinGame()));
    QObject::connect(abortAction, SIGNAL(triggered()), this, SLOT(abortLink()));
    QObject::connect(chessBoard, SIGNAL(clickPiece(QPair<int,int>)), this, SLOT(handleClick(QPair<int, int>)));
    QObject::connect(&server, SIGNAL(received(void*, int)), this, SLOT(handleReceive(void*, int)));
    QObject::connect(&client, SIGNAL(received(void*,int)), this, SLOT(handleReceive(void*, int)));
}

void MainWindow::createGame()
{
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
    createAction->setEnabled(1);
    joinAction->setEnabled(1);
    abortAction->setEnabled(0);
}

void MainWindow::handleClick(QPair<int, int> pos)
{
    if (lastPos.first && lastPos.second) {
        chessModel.movePiece(player, lastPos, pos);
        chessBoard->changeStatus(chessModel.getStatus(player));
        if (player == 'A')
            server.sendData((char*)&chessModel, sizeof(chessModel));
        else
            client.sendData((char*)&chessModel, sizeof(chessModel));
        lastPos = qMakePair(0, 0);
    } else {
        lastPos = pos;
    }
}

void MainWindow::handleReceive(void *buf, int len)
{
    if (len == sizeof(ChessModel)) {
        chessModel = *(ChessModel*)buf;
        chessBoard->changeStatus(chessModel.getStatus(player));
        delete (ChessModel*)buf;
    }
}

