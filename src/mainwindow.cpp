#include "mainwindow.h"
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setCentralWidget(new QWidget(this));
    mainLayout = new QHBoxLayout();
    centralWidget()->setLayout(mainLayout);

    chessBoard = new ChessBoard(centralWidget());
    mainLayout->addWidget(chessBoard);

    rightLayout = new QVBoxLayout();
    mainLayout->addLayout(rightLayout);

    //Create a space button to test
    rightLayout->addWidget(new QPushButton("space", this));
}

