#include "game.h"
#include "display.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_set>
using namespace sf;

typedef std::unordered_set<PieceSprite> psSet;

/////////////// PieceSprite definition ////////////////
PieceSprite::PieceSprite(int row, int col) {
    r = row;
    c = col;
}

PieceSprite::PieceSprite(int row, int col, Sprite spr) {
    r = row;
    c = col;
    sp = spr;
}

void PieceSprite::draw_figure(RenderWindow *window) const {
    window->draw(sp);
}

//////////////// DisplayGame definition ////////////////

DisplayGame::DisplayGame() {
    pieceSet = psSet();

    pieces = Texture(), board = Texture();
    pieces.loadFromFile("images/figures.png");
    board.loadFromFile("images/board.png");
    boardSprite = Sprite(board);

    window.create(VideoMode(boardSize, boardSize), "CHESS (press SPACE)");

    init_setup();
}

void DisplayGame::init_setup() {
    int pawnRows[2] = { 1, 6 };
    int otherRows[2] = { 0, 7 };

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j++) {
            Sprite tmpSp = Sprite(pieces, IntRect(fSize*figuresIndex[PAWN], fSize*(1-j), fSize, fSize));
            tmpSp.setPosition(fSize*i + hSize, fSize*(7-pawnRows[j]) + hSize);

            PieceSprite ps(pawnRows[j], i, tmpSp);
            pieceSet.insert(ps);

            tmpSp = Sprite(pieces, IntRect(fSize*figuresIndex[piecesOrder[i]], fSize*(1-j), fSize, fSize));
            tmpSp.setPosition(fSize*i + hSize, fSize*(7-otherRows[j]) + hSize);

            ps = PieceSprite(otherRows[j], i, tmpSp);
            pieceSet.insert(ps);
        }
    }
}

void DisplayGame::draw_game() {

    window.clear();
    window.draw(boardSprite);

    psSet::const_iterator it;
    Sprite *tmp;
    for (it = pieceSet.cbegin(); it != pieceSet.cend(); it++) {
        (*it).draw_figure(&window);
    }

    window.display();
}

void DisplayGame::apply_move(Point *src, Point *dst, bool isCastling) {
    PieceSprite ps(dst->r, dst->c);
    psSet::iterator it = pieceSet.find(ps);
    if (it != pieceSet.end()) pieceSet.erase(it);

    ps = PieceSprite(src->r, src->c);      
    it = pieceSet.find(ps);
    ps.sp = it->sp;
    pieceSet.erase(it);

    ps.r = dst->r;
    ps.c = dst->c;
    ps.sp.setPosition(fSize*(ps.c) + hSize, fSize*(7-(ps.r)) + hSize);
    pieceSet.insert(ps);


    if (isCastling) {
        int r = src->r ,c;

        if (dst->c > src->c) {
            c = 7;
        } else {
            c = 0;
        }

        ps = PieceSprite(r, c);
        it = pieceSet.find(ps);
        ps.sp = it->sp;
        pieceSet.erase(it);

        ps.r = src->r;
        ps.c = (c > 0) ? (dst->c - 1) : (dst->c + 1);
        ps.sp.setPosition(fSize*(ps.c) + hSize, fSize*(7-ps.r) + hSize);
        pieceSet.insert(ps);
    }
}