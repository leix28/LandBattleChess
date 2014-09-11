#include "chessmodel.h"

const QString ChessModel::pieceName[14] = {"", "司令", "军长", "师长", "旅长",
                                           "团长", "营长", "连长", "排长", "工兵",
                                           "地雷", "炸弹", "军旗", ""};

const int ChessModel::pieceType[25] = {1, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                                       7, 7, 7, 8, 8, 8, 9, 9, 9,
                                       10, 10, 10, 11, 11, 12};

ChessModel::ChessModel()
{
    posA[24] = qMakePair(1, 2);
    int p = 23;
    for (int i = 1; i <= 6; i++)
        for (int j = 1; j <= 5; j++) {
            if (i == 1 && j == 2) continue;
            if ((i == 3 || i == 5) && (j == 2 || j == 4)) continue;
            if (i == 4 && j == 3) continue;
            posA[p--] = qMakePair(i, j);
        }

    posB[24] = qMakePair(13, 4);
    p = 23;
    for (int i = 13; i >= 8; i--)
        for (int j = 5; j >= 1; j--) {
            if (i == 13 && j == 4) continue;
            if ((i == 9 || i == 11) && (j == 2 || j == 4)) continue;
            if (i== 10 && j == 3) continue;
            posB[p--] = qMakePair(i, j);
        }
}

ChessModel::ChessStatus ChessModel::getStatus(char player)
{
    ChessStatus ret;
    for (int i = 0; i < 25; i++) {
        if (posA[i].first && posA[i].second) {
            ret.push_back(qMakePair(posA[i], player == 'A' ? pieceType[i] : -13));
        }
        if (posB[i].first && posB[i].second) {
            ret.push_back(qMakePair(posB[i], player == 'B' ? pieceType[i] : -13));
        }
    }
    if (player == 'A') {
        for (int i = 0; i < ret.size(); i++) {
            ret[i].first.first = 14 - ret[i].first.first;
            ret[i].first.second = 6 - ret[i].first.second;
        }
    }
    return ret;
}

void ChessModel::movePiece(char player, QPair<int, int> st, QPair<int, int> ed)
{
    if (player == 'A') {
        st.first = 14 - st.first;
        st.second = 6 - st.second;

        ed.first = 14 - ed.first;
        ed.second = 6 - ed.second;
    }
    if (player == 'A') {
        for (int i = 0; i < 25; i++) {
            if (posA[i] == st) {
                posA[i] = ed;
            }
        }
    } else {
        for (int i = 0; i < 25; i++) {
            if (posB[i] == st) {
                posB[i] = ed;
            }
        }
    }
}
