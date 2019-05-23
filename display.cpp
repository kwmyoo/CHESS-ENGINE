#include "game.h"
#include "display.h"
#include <iostream>
#include <SFML/Graphics.hpp>
#include <unordered_set>
using namespace sf;

typedef std::unordered_set<PieceSprite, PSHash> psSet;

/////////////// PieceSprite definition ////////////////
PieceSprite::PieceSprite(int row, int col) {
    r = row;
    c = col;
}

PieceSprite::PieceSprite(int row, int col, Sprite *spr) {
    r = row;
    c = col;
    sp = spr;
}

void PieceSprite::change_member(int row, int col) {
    r = row, c = col;
}

void PieceSprite::change_member(int row, int col, Sprite *spr) {
    r = row, c = col;
    sp = spr;
}

bool PieceSprite::operator== (const PieceSprite& pt) const {
    return (r == pt.r) && (c == pt.c);
}

//////////////// DisplayGame definition ////////////////

DisplayGame::DisplayGame() {
    pieceSet = new psSet;
    window = new RenderWindow(VideoMode(boardSize, boardSize), "CHESS (press SPACE)");

    init_setup();
}

void DisplayGame::init_setup() {
    int pawnRows[2] = { 1, 6 };
    int otherRows[2] = { 0, 7 };
    Texture tmp;
    tmp.loadFromFile("images/figures2.png");
    const Texture pieces = tmp;

    Sprite *tmpSp;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 2; j++) {
            tmpSp = new Sprite(pieces, IntRect(size*figuresIndex[PAWN], size*j, size, size));
            //tmpSp.setTextureRect(IntRect(size*figuresIndex[PAWN], size*j, size, size));
            tmpSp->setPosition(size*i + hSize, size*(7-pawnRows[j]) + hSize);

            PieceSprite ps(pawnRows[j], i, tmpSp);
            pieceSet->insert(ps);

            tmpSp = new Sprite(pieces, IntRect(size*figuresIndex[piecesOrder[i]], size*j, size, size));
            //tmpSp->setTextureRect(IntRect(size*figuresIndex[piecesOrder[i]], size*j, size, size));
            tmpSp->setPosition(size*i + hSize, size*(7-otherRows[j]) + hSize);

            ps = PieceSprite(otherRows[j], i, tmpSp);
            pieceSet->insert(ps);
        }
    }
}

void DisplayGame::draw_game(const Sprite* boardSprite) {

    window->clear();
    window->draw(*boardSprite);

    psSet::iterator it;
    Sprite *tmp;
    for (it = pieceSet->begin(); it != pieceSet->end(); it++) {
        tmp = (*it).sp;
        window->draw(*tmp);
    }

    window->display();
}

void DisplayGame::apply_move(Point *src, Point *dst) {
    PieceSprite ps(dst->r, dst->c);
    psSet::iterator it = pieceSet->find(ps);
    if (it != pieceSet->end()) pieceSet->erase(it);

    ps = PieceSprite(src->r, src->c);      
    it = pieceSet->find(ps);
    ps.sp = it->sp;
    pieceSet->erase(it);

    ps.r = dst->r;
    ps.c = dst->c;
    ps.sp->setPosition(size*(ps.c) + hSize, size*(7-(ps.r)) + hSize);
    pieceSet->insert(ps);
}