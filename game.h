#ifndef GAME_H
#define GAME_H

#include <cstdlib>
#include <unordered_set>
#include <unordered_map>
using namespace std;

enum Side { WHITE, BLACK };
enum Piece { EMPTY, PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING };
enum P_type { HUMAN, ENGINE }; // player type

const Piece piecesOrder[] = { ROOK, KNIGHT, BISHOP, QUEEN, 
        KING , BISHOP, KNIGHT, ROOK }; // initial setup

struct Point {
    int r, c;
    bool operator< (const Point& pt) const {
        return (r < pt.r) || ((!(pt.r < r)) && (c < pt.c));
    }

    bool operator== (const Point& pt) const {
        return (r == pt.r) && (c == pt.c);
    }
};

class PointHash {
public:
    size_t operator() (const Point& pt) const {
        return pt.r*8 + pt.c;
    }
};

class Game {
public:
    int board[8][8];
    P_type wp; // white player
    P_type bp; // black player
    Side curr_turn;
    bool moved[2][3]; // row 0 1 : white black, col 0 1 2: king l_rook r_rook

    // array of set of positions for each pieces
    unordered_set<Point, PointHash> *whitePiece;
    unordered_set<Point, PointHash> *blackPiece;

    long white_timer;
    long black_timer;

    Game(P_type wType, P_type bType);
    bool is_valid_pick(Point *from);
    bool handle_command(Point *src, Point *dst); // return true on valid moves

private:
    Point src, dst; // for move command
    bool isCastling;

    void init_board();
    int check_first(Point p, int rd, int cd);
    bool not_blocked(bool forCastling);
    bool is_check();
    bool is_valid_castling();
    bool is_valid_move();
    void make_move();
    int result_check();
    void change_turn();
};

#endif