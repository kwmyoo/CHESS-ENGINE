#include "game.h"
#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

Game::Game(P_type wType, P_type bType) {
    wp = wType;
    bp = bType;

    curr_turn = WHITE;
    int i,j;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            moved[i][j] = false;
        }
    }

    whitePiece = new unordered_set<Point>[7];
    blackPiece = new unordered_set<Point>[7];

    init_board();
}

bool Game::is_valid_pick(Point *from) {
    int p = board[from->r][from->c];

    bool result;
    if (p > 0) result = (curr_turn == WHITE);
    else if (p < 0) result = (curr_turn == BLACK);
    else result = false;
    
    return result;
}

// initialize game board state
void Game::init_board() {
    int i;
    Point wPoint, bPoint;
    Piece p;

    for (i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = EMPTY;
        }
    }
    
    for (i = 0; i < 8; i++) { // iterate over column
        // add pawn
        wPoint.r = 1;
        wPoint.c = i;
        bPoint.r = 6;
        bPoint.c = i;

        whitePiece[PAWN].insert(wPoint);
        blackPiece[PAWN].insert(bPoint);
        board[wPoint.r][wPoint.c] = PAWN;
        board[bPoint.r][bPoint.c] = -PAWN;

        // add other pieces in same column
        p = piecesOrder[i];
        wPoint.r = 0;
        bPoint.r = 7;
        
        whitePiece[p].insert(wPoint);
        blackPiece[p].insert(bPoint);
        board[wPoint.r][wPoint.c] = p;
        board[bPoint.r][bPoint.c] = -p;
    }
}

inline int Game::check_first(Point pt, int rd, int cd) {
    rd = (rd == 0) ? 0 : rd/abs(rd);
    cd = (cd == 0) ? 0 : cd/abs(cd);
    int r = pt.r + rd, c = pt.c + cd;
    int p;
    while (r <= 7 && c <= 7) {
        p = board[r][c];
        if (p != EMPTY) return p;

        r += rd;
        c += cd;
    }

    return 0;
}

// return true if the path between src and dst is unblocked
// assume the input points are positioned vertical, horizontal or diagonal
// third parameter is set to true when it is used for checking for castling
inline bool Game::not_blocked(bool forCastling, Point *source, Point *dest) {
    int rDiff = dest->r - source->r; // vectors for stepping
    int cDiff = dest->c - source->c;
    if (rDiff != 0) rDiff /= abs(rDiff);
    if (cDiff != 0) cDiff /= abs(cDiff);

    int r = source->r + rDiff;
    int c = source->c + cDiff;

    Side srcSide = (board[source->r][source->c] > 0) ? WHITE : BLACK;
    Side dstSide = (board[dest->r][dest->c] > 0) ? WHITE : BLACK;

    while ((r != dest->r) || (c != dest->c)) {
        if (board[r][c] != EMPTY) return false;

        r += rDiff;
        c += cDiff;
    }

    if (!forCastling && (board[dest->r][dest->c] != EMPTY) && (srcSide == dstSide))
        return false;
    
    return true;
}

// TODO!!!!
// if true, checks whether the move caused same side king's check
// if false, checks for opponent's king
inline bool Game::is_check() {
    Point kingPos;
    unordered_set<Point>::iterator it;
    int rDiff, cDiff, s, t, p = 0;

    it = (board[src.r][src.c] > 0) ? whitePiece[KING].begin() :
        blackPiece[KING].begin();
    kingPos = *it;
    rDiff = src.r - kingPos.r;
    cDiff = src.c - kingPos.c;

    if (!not_blocked(false, &src, &kingPos))

    s = (curr_turn == WHITE) ? -1 : 1;

    if (rDiff == 0 || cDiff == 0) {
        p = check_first(src, rDiff, cDiff);
        t = ROOK;
    } else if (abs(rDiff) == abs(cDiff)) {
        p = check_first(src, rDiff, cDiff);
        t = BISHOP;
    }
    p *= s;

    if (p == t || p == QUEEN) {
        return true;
    }

    return false;
}

// check whether the given input points represent valid castling
bool Game::is_valid_castling() {
    int target = board[src.r][src.c];
    Point rookPos;
    rookPos.r = src.r;

    Side s = (target > 0) ? WHITE : BLACK;

    // check whether the king has moved or not
    if (moved[s][0]) return false;
        
    // check for rook's validity
    if (src.c > dst.c) { // left rook
        if (moved[s][1]) return false;
        rookPos.c = 0;
    } else { // right rook
        if (moved[s][2]) return false;
        rookPos.c = 7;
    }

    if (!not_blocked(true, &src, &dst)) return false;

    return true;
}

bool Game::is_valid_move() {
    if ((src.r == dst.r) && (src.c == dst.c)) return false;
    int piece = board[src.r][src.c], tmp, dstPiece;
    int rDiff = abs(dst.r - src.r);
    int cDiff = abs(dst.c - src.c);
    Side s = (piece > 0) ? WHITE : BLACK;
    Side dstSide;

    if (s != curr_turn) return false;

    switch (abs(piece)) {
        case PAWN: // this case, piece = (valid vertical moving size)

            // pawn moving vertically
            if ((dst.c == src.c) && (board[dst.r][dst.c] == EMPTY)) {
                if (dst.r == src.r + piece) {} // one step
                else if (dst.r == src.r + 2*piece) { // two steps
                    tmp = (s == WHITE) ? 1 : 6;
                    
                    // if blocked or not in valid position
                    if ((src.r != tmp) 
                            || (board[src.r + piece][src.c] != EMPTY))
                        return false;
                }
                else return false;
            
            // moving diagonally
            } else if ((dst.r == src.r + piece)
                    && (cDiff == 1)) {
                if (board[dst.r][dst.c] == EMPTY) return false;

            // if none of above, return false
            } else return false;
            
            break;

        case KNIGHT:
            dstPiece = board[dst.r][dst.c];
            dstSide = (dstPiece > 0) ? WHITE : BLACK;

            if (!(rDiff == 2 && cDiff == 1) && !(rDiff == 1 && cDiff == 2)) return false;

            // if same side piece is on dst
            if ((dstPiece != EMPTY) && (dstSide == s)) return false;
            
            break;
        
        case BISHOP:
            if (rDiff != cDiff) return false;
            if (!not_blocked(false, &src, &dst)) return false;
            
            break;

        case ROOK:
            if ((src.r != dst.r) && (src.c != dst.c)) return false;
            if (!not_blocked(false, &src, &dst)) return false;

            break;

        case QUEEN:
            if ((rDiff != cDiff)
                && ((src.r != dst.r) && (src.c != dst.c))) return false;

            if (!not_blocked(false, &src, &dst)) return false;

            break;

        case KING:
            tmp = rDiff + cDiff;

            // king moving one step
            if (tmp == 1) {
                dstPiece = board[dst.r][dst.c];
                dstSide = (dstPiece > 0) ? WHITE : BLACK;
                if ((dstPiece != EMPTY) && (dstSide == s)) return false;
            
            // if valid castling
            } else if ((tmp == 2) && is_valid_castling()) {
                isCastling = true;
            } else return false;

            break;
    }

    // if the move results in same side king's check
    if (is_check()) return false;

    return true;
}

// apply all changes to the game state made by the move
void Game::make_move() {
    // first change the point set for pieces
    int p = board[src.r][src.c];
    if (curr_turn == WHITE) {
        whitePiece[abs(p)].erase(src);
        whitePiece[abs(p)].insert(dst);
    } else {
        blackPiece[abs(p)].erase(src);
        blackPiece[abs(p)].insert(dst);        
    }

    int target = board[dst.r][dst.c];
    if (target != EMPTY) {
        if (curr_turn == WHITE) {
            blackPiece[abs(target)].erase(dst);
        } else {
            whitePiece[abs(target)].erase(dst);
        }
    }

    board[dst.r][dst.c] = board[src.r][src.c];
    board[src.r][src.c] = EMPTY;

    if (isCastling) {
        if (dst.c < src.c) {
            board[src.r][dst.c+1] = board[src.r][0];
            board[src.r][0] = EMPTY;
        } else {
            board[src.r][dst.c-1] = board[src.r][7];
            board[src.r][7] = EMPTY;            
        }
    }

    if (p == KING) { // if white king moved
        moved[WHITE][0] = true;
    } else if (p == -KING) {
        moved[BLACK][0] = true;
    } else if (p == ROOK) {
        if (src.r == 0) {
            if (src.c == 0) moved[WHITE][1] = true;
            else if (src.c == 7) moved[WHITE][2] = true;
        }
    } else if (p == -ROOK) {
        if (src.r == 7) {
            if (src.c == 0) moved[BLACK][1] = true;
            else if (src.c == 7) moved[BLACK][2] = true;
        }
    }
}

void Game::change_turn() {
    curr_turn = (curr_turn == WHITE) ? BLACK : WHITE;
}

// c
int Game::result_check() {

}

bool Game::handle_command(Point *source, Point *dest) {
    // set class data for processing command
    src = *source;
    dst = *dest;
    isCastling = false;

    int oppStatus;
    bool isValid;
    if (isValid = is_valid_move()) {
        make_move();
        oppStatus = result_check();
        change_turn();

    }

    return isValid;
}