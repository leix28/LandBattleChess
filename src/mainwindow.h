#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "chessboard.h"
#include "chessmodel.h"
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


public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

};

#endif // MAINWINDOW_H
