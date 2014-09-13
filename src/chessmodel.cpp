#include "chessmodel.h"
#include <QQueue>
#include <QSet>
#include <QDebug>

const QString ChessModel::pieceName[14] = {"", "司令", "军长", "师长", "旅长",
                                           "团长", "营长", "连长", "排长", "工兵",
                                           "地雷", "炸弹", "军旗", ""};

const int ChessModel::pieceType[25] = {1, 2, 3, 3, 4, 4, 5, 5, 6, 6,
                                       7, 7, 7, 8, 8, 8, 9, 9, 9,
                                       10, 10, 10, 11, 11, 12};

const int ChessModel::dx[8] = {0, 1, 0, -1, 1, -1, 1, -1};
const int ChessModel::dy[8] = {1, 0, -1, 0, 1, -1, -1, 1};

int ChessModel::getChessId(char player, QPair<int, int> pos) const
{
    for (int i = 0; i < 25; i++) {
        if (player == 'A' && posA[i] == pos)
            return i;
        if (player == 'B' && posB[i] == pos)
            return i;
    }
    return -1;
}

bool ChessModel::isHouse(QPair<int, int> pos) const
{
    if ((pos.first == 3 || pos.first == 5 || pos.first == 9 || pos.first == 11) &&
            (pos.second == 2 || pos.second == 4)) return 1;
    if ((pos.first == 4 || pos.first == 10) && pos.second == 3) return 1;
    return 0;
}

bool ChessModel::isRail(QPair<int, int> pos) const
{
    if (pos.first < 1 || pos.first > 13 || pos.second < 1 || pos.second > 5) return 0;
    if (pos.first == 2 || pos.first == 6 || pos.first == 8 || pos.first == 12) return 1;
    if ((2 <= pos.first && pos.first <= 12) && (pos.second == 1 || pos.second == 5)) return 1;
    if ((6 <= pos.first && pos.first <= 8) && (pos.second == 3)) return 1;
    return 0;
}

bool ChessModel::checkPos(int piece, QPair<int, int> pos) const
{
    if (piece == 12)
        return (pos.first == 1 || pos.first == 13) && (pos.second == 2 || pos.second == 4);
    if (piece == 11)
        return pos.first != 6 && pos.first != 8;
    if (piece == 10)
        return pos.first <= 2 || pos.first >= 11;
    return 1;
}

ChessModel::ChessModel()
{
    initStatus();
}

void ChessModel::initStatus() {
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

ChessModel::ChessStatus ChessModel::getStatus(char player) const
{
    ChessStatus ret;
    for (int i = 0; i < 25; i++) {
        if (posA[i].first && posA[i].second) {
            //ret.push_back(qMakePair(posA[i], player == 'A' ? pieceType[i] : -13));
            ret.push_back(qMakePair(posA[i], player == 'A' ? pieceType[i] : -pieceType[i]));
                    }
        if (posB[i].first && posB[i].second) {
            //ret.push_back(qMakePair(posB[i], player == 'B' ? pieceType[i] : -13));
            ret.push_back(qMakePair(posB[i], player == 'B' ? pieceType[i] : -pieceType[i]));
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
        int x = getChessId('A', st), y = getChessId('B', ed);
        posA[x] = ed;
        if (x != -1 && y != -1) {
            if (!win(x, y))
                posA[x] = qMakePair(0, 0);
            if (!win(y, x))
                posB[x] = qMakePair(0, 0);
        }
    } else {
        int x = getChessId('A', ed), y = getChessId('B', st);
        posB[y] = ed;
        if (x != -1 && y != -1) {
            if (!win(x, y))
                posA[x] = qMakePair(0, 0);
            if (!win(y, x))
                posB[y] = qMakePair(0, 0);
        }
    }
}

bool ChessModel::isMovable(char player, QPair<int, int> st, QPair<int, int> ed) const
{
    if (player == 'A') {
        st.first = 14 - st.first;
        st.second = 6 - st.second;
        ed.first = 14 - ed.first;
        ed.second = 6 - ed.second;
    }
    if (st.first < 1 || st.first > 13 || st.second < 1 || st.second > 5 ||
            ed.first < 1 || ed.first > 13 || ed.second < 1 || ed.second > 5) return 0;
    if (getChessId(player, ed) != -1) return 0;
    if (getChessId(player, st) == -1) return 0;
    if (isHouse(ed) && (getChessId('A' + 'B' - player, ed) != -1)) return 0;

    int pt = pieceType[getChessId(player, st)];
    if (pt == 10 || pt == 12) return 0;

    if (ed.first == 7  && (ed.second == 2 || ed.second == 4)) return 0;
    if ((st.first == 1 || st.first == 13) && (st.second == 2 || st.second == 4)) return 0;
    int dis = abs(st.first - ed.first) + abs(st.second - ed.second);

    if (dis == 1) return 1;
    if (dis == 2 && (isHouse(st) || isHouse(ed))) return 1;

    if (isRail(st) && isRail(ed)) {
        if (st.first == ed.first) {
            bool flag = 1;
            for (int i = qMin(st.second, ed.second) + 1; i <= qMax(st.second, ed.second) - 1; i++)
                if (getChessId('A', qMakePair(st.first, i)) != -1 || getChessId('B', qMakePair(st.first, i)) != -1)
                    flag = 0;
            if (flag) return 1;
        }
        if (st.second == ed.second) {
            bool flag = 1;
            for (int i = qMin(st.first, ed.first) + 1; i <= qMax(st.first, ed.first) - 1; i++)
                if (getChessId('A', qMakePair(i, st.second)) != -1 || getChessId('B', qMakePair(i, st.second)) != -1)
                    flag = 0;
            if (flag) return 1;
        }
        if (pt != 9) return 0;
        QQueue< QPair<int, int> > queue;
        QSet< QPair<int, int> > set;
        queue.push_back(st);
        while (!queue.empty()) {
            QPair<int, int> now = queue.front();
            queue.pop_front();
            for (int i = 0; i < 4; i++) {
                QPair<int, int> tmp = now;
                tmp.first += dx[i];
                tmp.second += dy[i];
                if (isRail(tmp) && getChessId('A', tmp) == -1 && getChessId('B', tmp) == -1 && set.find(tmp) == set.end()) {
                    queue.push_back(tmp);
                    set.insert(tmp);
                }
            }
        }
        for (auto itm : set) {
            if (qAbs(itm.first - ed.first) + qAbs(itm.second - ed.second) <= 1)
                return 1;
        }
    }
    return 0;
}

bool ChessModel::isOwner(char player, QPair<int, int> pos) const
{
    if (player == 'A') {
        pos.first = 14 - pos.first;
        pos.second = 6 - pos.second;
    }
    for (int i = 0; i < 25; i++) {
        if (player == 'A' && posA[i] == pos)
            return 1;
        if (player == 'B' && posB[i] == pos)
            return 1;
    }
    return 0;
}

bool ChessModel::win(int x, int y) const
{
    x = pieceType[x];
    y = pieceType[y];
    if (x == 12) return 0;
    if (x == 11) return 0;
    if (y == 12) return 1;
    if (y == 11) return 0;
    if (y == 10) return x == 9;
    if (x == 10) return y != 9;
    return x < y;
}

void ChessModel::swapPiece(char player, QPair<int, int> st, QPair<int, int> ed)
{
    if (player == 'A') {
        st.first = 14 - st.first;
        st.second = 6 - st.second;
        ed.first = 14 - ed.first;
        ed.second = 6 - ed.second;
    }
    int x = getChessId(player, st), y = getChessId(player, ed);
    if (player == 'A')
        qSwap(posA[x], posA[y]);
    else
        qSwap(posB[x], posB[y]);
}

bool ChessModel::isSwapable(char player, QPair<int, int> st, QPair<int, int> ed) const
{
    if (player == 'A') {
        st.first = 14 - st.first;
        st.second = 6 - st.second;
        ed.first = 14 - ed.first;
        ed.second = 6 - ed.second;
    }
    int x = getChessId(player, st), y = getChessId(player, ed);
    if (x == -1 || y == -1) return 0;
    return checkPos(pieceType[x], ed) && checkPos(pieceType[y], st);
}

void ChessModel::setStatus(char player, ChessModel model)
{
    if (player == 'A')
        for (int i = 0; i < 25; i++)
            posA[i] = model.posA[i];
    if (player == 'B')
        for (int i = 0; i < 25; i++)
            posB[i] = model.posB[i];
}

bool ChessModel::isWin(char player)
{
    if (player == 'A') {
        if (!posB[24].first || !posB[24].second) {
            return 1;
        }
        for (int i = 0; i < 25; i++)
            for (int j = 0; j < 8; j++) {
                QPair<int, int> tmp = posB[i];
                tmp.first += dx[j];
                tmp.second += dy[j];
                if (isMovable('B', posB[i], tmp)) return 0;
            }
        return 1;
    } else {
        if (!posA[24].first || !posA[24].second) {
            qDebug() << "FAIL";
            return 1;
        }
        for (int i = 0; i < 25; i++)
            for (int j = 0; j < 8; j++) {
                QPair<int, int> tmp = posA[i], tt;
                tt = posA[i];
                tt.first = 14 - tt.first;
                tt.second = 6 - tt.second;
                tmp.first += dx[j];
                tmp.second += dy[j];
                tmp.first = 14 - tmp.first;
                tmp.second = 6 - tmp.second;
                if (isMovable('A', tt, tmp)) return 0;
            }
        return 1;
    }
}
