#include "mainwindow.h"
#include "guessdialog.h"
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <cstring>
#include <QLabel>

#define BUFSIZE 1000

void MainWindow::serverSend(QByteArray a, char pre)
{
    int len = a.size();
    char buf[BUFSIZE + 1];
    memset(buf, 0, BUFSIZE + 1);
    buf[0] = pre;
    memcpy(buf + 1, &len, 4);
    server.sendData(buf, BUFSIZE + 1);
    for (int i = 0; i <= a.size() / BUFSIZE; i++) {
        memset(buf, 0, BUFSIZE + 1);
        buf[0] = pre;
        memcpy(buf + 1, a.data() + i * BUFSIZE, i == a.size() / BUFSIZE ? a.size() % BUFSIZE : BUFSIZE);
        server.sendData(buf, BUFSIZE + 1);
    }
}

void MainWindow::clientSend(QByteArray a, char pre)
{
    int len = a.size();
    char buf[BUFSIZE + 1];
    memset(buf, 0, BUFSIZE + 1);
    buf[0] = pre;
    memcpy(buf + 1, &len, 4);
    client.sendData(buf, BUFSIZE + 1);
    for (int i = 0; i <= a.size() / BUFSIZE; i++) {
        memset(buf, 0, BUFSIZE + 1);
        buf[0] = pre;
        memcpy(buf + 1, a.data() + i * BUFSIZE, i == a.size() / BUFSIZE ? a.size() % BUFSIZE : BUFSIZE);
        client.sendData(buf, BUFSIZE + 1);
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    server(BUFSIZE + 1, this),
    client(BUFSIZE + 1, this)
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

    opp = new UserWidget(this);
    loc = new UserWidget(this);
    oppName = new QLabel(this);
    locName = new QLabel(this);
    QVBoxLayout *rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout);

    rightLayout->addWidget(opp);
    rightLayout->addWidget(oppName);
    QPushButton *startButton = new QPushButton("Start", this);
    rightLayout->addWidget(startButton);
    rightLayout->addWidget(loc);
    rightLayout->addWidget(locName);

    //TODO rightLayout for some information


    QObject::connect(createAction, SIGNAL(triggered()), this, SLOT(createGame()));
    QObject::connect(joinAction, SIGNAL(triggered()), this, SLOT(joinGame()));
    QObject::connect(abortAction, SIGNAL(triggered()), this, SLOT(abortLink()));
    QObject::connect(chessBoard, SIGNAL(clickPiece(QPair<int,int>)), this, SLOT(handleClick(QPair<int, int>)));
    QObject::connect(&server, SIGNAL(received(void*, int)), this, SLOT(handleReceive(void*, int)));
    QObject::connect(&server, SIGNAL(connected()), this, SLOT(handleConnected()));
    QObject::connect(&client, SIGNAL(received(void*,int)), this, SLOT(handleReceive(void*, int)));
    QObject::connect(startButton, SIGNAL(clicked()), this, SLOT(startGame()));
}

void MainWindow::createGame()
{
    u = 0; lenu = 0; i = 0; leni = 0; pu = pi = 0;
    locUser.clear(); oppUser.clear();
    isStart = 0;
    lastPos = qMakePair(0, 0);

    QString ret = QInputDialog::getText(this, QString("IP"), QString("Local IP: "), QLineEdit::Normal, QString("127.0.0.1"));
    char buf[20];
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i < ret.size(); i++)
        buf[i] = ret[i].unicode();
    if (server.closeServer() &&
            client.closeClient() &&
            server.initServer(buf, 4321) &&
            server.startServer()) {
        statusBar()->showMessage("Server Created.");
        createAction->setEnabled(0);
        joinAction->setEnabled(0);
        abortAction->setEnabled(1);

    } else
        statusBar()->showMessage("Fail: create");
    player = 'A';
    locUser.load();
    locName->setText(locUser.name);
    loc->paintImage(locUser.image);
}

void MainWindow::joinGame()
{
    u = 0; lenu = 0; i = 0; leni = 0; pu = pi = 0;
    locUser.clear(); oppUser.clear();
    isStart = 0;
    lastPos = qMakePair(0, 0);

    QString ret = QInputDialog::getText(this, QString("IP"), QString("Server IP: "), QLineEdit::Normal, QString("127.0.0.1"));
            char buf[20];
    memset(buf, 0, sizeof(buf));
    for (int i = 0; i < ret.size(); i++)
        buf[i] = ret[i].unicode();
    if (server.closeServer() &&
            client.closeClient() &&
            client.connectServer(buf, 4321)) {
        statusBar()->showMessage("Server connected.");
        createAction->setEnabled(0);
        joinAction->setEnabled(0);
        abortAction->setEnabled(1);
    } else
        statusBar()->showMessage("Fail: connect.");
    player = 'B';
    locUser.load();
    clientSend(locUser.name.toLatin1(), 'U');
    clientSend(locUser.image, 'I');
    locName->setText(locUser.name);
    loc->paintImage(locUser.image);
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
        QPair<int, int> tt = pos;
        tt.first = 14 - tt.first;
        tt.second = 6 - tt.second;
        if ((!lastPos.first || !lastPos.second) && chessModel.isOwner('A' + 'B' - player, tt)) {
            int type = GuessDialog::getGuess(this);
            if (type != 0 && type != 13)
                chessModel.guess(player, pos, type);
            chessBoard->changeStatus(chessModel.getStatus(player));
            char buf[BUFSIZE + 1];
            memset(buf, 0, BUFSIZE + 1);
            buf[0] = player - 'A' + 'a';
            memcpy(buf + 1, &chessModel, sizeof(ChessModel));
            if (player == 'A') {
                server.sendData(buf, BUFSIZE + 1);
            } else {
                client.sendData(buf, BUFSIZE + 1);
            }
        }
        if (isWaiting) return;
        if (lastPos.first && lastPos.second) {
            if (chessModel.isMovable(player, lastPos, pos)) {
                isWaiting = 1;
                statusBar()->showMessage("Waiting");
                chessModel.movePiece(player, lastPos, pos);
                chessBoard->changeStatus(chessModel.getStatus(player));
                char buf[BUFSIZE + 1];
                memset(buf, 0, BUFSIZE + 1);
                buf[0] = 0;
                memcpy(buf + 1, &chessModel, sizeof(ChessModel));
                if (player == 'A')
                    server.sendData(buf, BUFSIZE + 1);
                else
                    client.sendData(buf, BUFSIZE + 1);
                lastPos = qMakePair(0, 0);
                if (chessModel.isWin(player)) {
                    QMessageBox message;
                    message.setText("You Win!");
                    message.exec();
                    statusBar()->showMessage(QString("You Win!"));
                }
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
        if (lastPos.first && lastPos.second) {
            if (chessModel.isSwapable(player, lastPos, pos)) {
                chessModel.swapPiece(player, lastPos, pos);
                lastPos = qMakePair(0, 0);
                statusBar()->showMessage(QString("Please Organize Your Chess Pieces"));
                chessBoard->changeStatus(chessModel.getStatus(player));
            } else {
                if (chessModel.isOwner(player, pos)) {
                    lastPos = pos;
                    statusBar()->showMessage(QString("Select (%1, %2)").arg(pos.first).arg(pos.second));
                } else {
                    lastPos = qMakePair(0, 0);
                    statusBar()->showMessage(QString("Please Organize Your Chess Pieces"));
                }
            }
        } else {
            if (chessModel.isOwner(player, pos)) {
                lastPos = pos;
                statusBar()->showMessage(QString("Select (%1, %2)").arg(pos.first).arg(pos.second));
            } else {
                lastPos = qMakePair(0, 0);
                statusBar()->showMessage(QString("Please Organize Your Chess Pieces"));
            }
        }
    }
}

void MainWindow::handleReceive(void *bufv, int len)
{
    if (len != BUFSIZE + 1) {
        qDebug() << "War";
    }
    char *buf = (char*)bufv;

    if (buf[0] == 0) {
        if (isStart) {
            statusBar()->showMessage(QString("Your Trun"));
            isWaiting = 0;
        } else
            statusBar()->showMessage(QString("Please Organize Your Chess Pieces"));
        memcpy(&chessModel, buf + 1, sizeof(ChessModel));
        chessBoard->changeStatus(chessModel.getStatus(player));
        if (chessModel.isWin('A' + 'B' - player)) {
            isWaiting = 1;
            QMessageBox message;
            message.setText("You Lose!");
            message.exec();
            statusBar()->showMessage(QString("You Lose!"));
        }
    } else if (buf[0] == 'A' || buf[0] == 'B') {
        ChessModel now;
        memcpy(&now, buf + 1, sizeof(ChessModel));
        chessModel.setStatus(buf[0], now);
        if (isStart) {
            isWaiting = 0;
            statusBar()->showMessage("Your Turn");
        } else {
            statusBar()->showMessage("Your Opponent is Ready");
        }
    } else if (buf[0] == 'a' || buf[0] == 'b') {
        ChessModel now;
        memcpy(&now, buf + 1, sizeof(ChessModel));
        chessModel.setGuess(buf[0] - 'a' + 'A', now);
    }

    if (buf[0] == 'U') {
        if (!u) {
            memcpy(&lenu, buf + 1, 4);
            u = new char[lenu];
        } else {
            for (int i = 1; i <= BUFSIZE && pu < lenu ; i++)
                u[pu++] = buf[i];
            if (pu == lenu) {
                oppUser.name = QString().fromLatin1(QByteArray(u, lenu));
                oppName->setText(oppUser.name);
            }
        }
    } else if (buf[0] == 'I') {
        if (!i) {
            memcpy(&leni, buf + 1, 4);
            i = new char[leni];
        } else {
            for (int x = 1; x <= BUFSIZE && pi < leni ; x++)
                i[pi++] = buf[x];
            if (pi == leni) {
                oppUser.image = QByteArray(i, leni);
                opp->paintImage(oppUser.image);
            }
        }
    }
    delete buf;
}

void MainWindow::handleConnected()
{
    serverSend(locUser.name.toLatin1(), 'U');
    serverSend(locUser.image, 'I');

    chessModel.initStatus();
    chessBoard->changeStatus(chessModel.getStatus(player));

    char buf[BUFSIZE + 1];
    memset(buf, 0, BUFSIZE + 1);
    buf[0] = 0;
    memcpy(buf + 1, &chessModel, sizeof(ChessModel));
    server.sendData(buf, BUFSIZE + 1);
    statusBar()->showMessage(QString("Please Organize Your Chess Pieces"));
}

void MainWindow::startGame()
{
    lastPos = qMakePair(0, 0);
    isStart = 1; isWaiting = 1;
    char *buf = new char[sizeof(ChessModel) + 1];
    buf[0] = player;
    memcpy(buf + 1, &chessModel, sizeof(ChessModel));
    if (player == 'A')
        server.sendData(buf, sizeof(ChessModel) + 1);
    else
        client.sendData(buf, sizeof(ChessModel) + 1);
    statusBar()->showMessage("Waiting");

}

