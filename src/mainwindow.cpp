#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
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


    setCentralWidget(new QWidget(this));
    QHBoxLayout *mainLayout = new QHBoxLayout();
    centralWidget()->setLayout(mainLayout);

    chessBoard = new ChessBoard(centralWidget());
    mainLayout->addWidget(chessBoard);

    QVBoxLayout *rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout);

    //TODO rightLayout for some information

    chessBoard->changeStatus(chessModel.getStatus('B'));
}

