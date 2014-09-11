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
    QAction *createAction = new QAction("Create", gameMenu);
    createAction->setIcon(QIcon(":/create.icns"));
    QAction *joinAction = new QAction("Join in", gameMenu);
    joinAction->setIcon(QIcon(":/join.icns"));
    menuBar()->addMenu(gameMenu);
    gameMenu->addAction(createAction);
    gameMenu->addAction(joinAction);
    QToolBar *toolbar = new QToolBar(this);
    toolbar->setMovable(0);
    addToolBar(Qt::LeftToolBarArea, toolbar);
    toolbar->addAction(createAction);
    toolbar->addAction(joinAction);

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
    QObject::connect(chessBoard, SIGNAL(clickPiece(QPair<int,int>)), this, SLOT(handleClick(QPair<int, int>)));
    QObject::connect(&server, SIGNAL(received(void*, int)), this, SLOT(handleReceive(void*, int)));
    QObject::connect(&client, SIGNAL(received(void*,int)), this, SLOT(handleReceive(void*, int)));
}

void MainWindow::createGame()
{
    if (server.closeServer() &&
            client.closeClient() &&
            server.initServer((char*)"127.0.0.1", 4321) &&
            server.startServer()) {
        statusBar()->showMessage("Server Created.");
    } else
        statusBar()->showMessage("Fail: create");
    player = 'A';
}

void MainWindow::joinGame()
{
    if (server.closeServer() &&
            client.closeClient() &&
            client.connectServer((char*)"127.0.0.1", 4321)) {
        statusBar()->showMessage("Server connected.");
    } else
        statusBar()->showMessage("Fail: connect.");
    player = 'B';

}

void MainWindow::handleClick(QPair<int, int> pos)
{
    char buf[100];
    sprintf(buf, "Player %c click (%d, %d)\n", player, pos.first, pos.second);
    client.sendData(buf, strlen(buf));
}

void MainWindow::handleReceive(void *bufv, int len)
{
    statusBar()->showMessage(QString((char*)bufv));
    delete bufv;
}

