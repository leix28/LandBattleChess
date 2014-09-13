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
    const static int dx[8];
    const static int dy[8];
    QPair<int, int> posA[25];
    QPair<int, int> posB[25];
    int getChessId(char player, QPair<int, int> pos) const;
    bool isHouse(QPair<int, int> pos) const ;
    bool isRail(QPair<int, int> pos) const;
    bool checkPos(int piece, QPair<int, int> pos) const;

public:
    typedef QVector< QPair< QPair<int, int>, int > > ChessStatus;
    ChessModel();
    void initStatus();
    const static QString pieceName[14];
    const static int pieceType[25];
    ChessStatus getStatus(char player) const;
    void movePiece(char player, QPair<int, int> st, QPair<int, int> ed);
    bool isMovable(char player, QPair<int, int> st, QPair<int, int> ed) const;
    bool isOwner(char player, QPair<int, int> pos) const;
    bool win(int x, int y) const;
    void swapPiece(char player, QPair<int, int> st, QPair<int, int> ed);
    bool isSwapable(char player, QPair<int, int> st, QPair<int, int> ed) const;
    void setStatus(char player, ChessModel model);
    bool isWin(char player);
};

#endif // CHESSMODEL_H
