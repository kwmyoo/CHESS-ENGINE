#ifndef DISPLAY_H
#define DISPLAY_H

#include "game.h"
#include <SFML/Graphics.hpp>
#include <unordered_set>
using namespace sf;

class PieceSprite {
public:
    int r, c;
    Sprite sp;

    PieceSprite(int row, int col);
    PieceSprite(int row, int col, Sprite spr);

    void draw_figure(RenderWindow *window) const;

    bool operator== (const PieceSprite& pt) const {
        return (pt.r == r) && (pt.c == c);
    };
};

namespace std {
    template<> struct hash<PieceSprite> {
        size_t operator() (const PieceSprite& pt) const {
            return pt.r*8 + pt.c;
        }
    };
}

class DisplayGame {
public:
    const int boardSize = 504;
    const int size = 56;
    const int hSize = size/2;
    RenderWindow *window;

    DisplayGame();

    void apply_move(Point *src, Point *dst);
    void draw_game(const Sprite* bds);

private:
    std::unordered_set<PieceSprite> *pieceSet;
    Texture *pieces;

    void init_setup();

    const int figuresIndex[7] = { 0, 5, 1, 2, 0, 3, 4 };
};

#endif