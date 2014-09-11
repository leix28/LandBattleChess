#ifndef CHESSMODEL_H
#define CHESSMODEL_H

#include <QString>
#include <QVector>
#include <QPair>

//player A Row 1 to 6
//player B Row 8 to 13
//first-Row second-Column
class ChessModel
{
    QPair<int, int> posA[25];
    QPair<int, int> posB[25];

public:
    typedef QVector< QPair< QPair<int, int>, int > > ChessStatus;
    ChessModel();
    const static QString pieceName[14];
    const static int pieceType[25];
    ChessStatus getStatus(char player);
    void movePiece(char player, QPair<int, int> st, QPair<int, int> ed);
};

#endif // CHESSMODEL_H
