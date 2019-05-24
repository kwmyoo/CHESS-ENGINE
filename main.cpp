#include "game.h"
#include "display.h"
#include <iostream>
#include <string>

// from display.h, size of figures and board
int hSize, fSize;

void store_mouse_pos(sf::Vector2i pos, Point *pt) {
    int c = (pos.x - hSize) / fSize;
    int r = 7 - (pos.y - hSize) / fSize;

    pt->r = r;
    pt->c = c;
}

int main() {
    
    Game g(HUMAN, HUMAN);
    DisplayGame display;
    hSize = display.hSize, fSize = display.fSize;

    Point *src = new Point, *dst = new Point;
    bool srcSet = false;
    int r, c;

    sf::Vector2i pos;
    sf::Event e;

    while (display.window.isOpen()) {
        display.draw_game();

        while (display.window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) display.window.close();

            if (e.type == sf::Event::MouseButtonReleased) {
                pos = sf::Mouse::getPosition(display.window);
                
                if (e.mouseButton.button == sf::Mouse::Left) {
                    if (srcSet) {
                        store_mouse_pos(pos, dst);
                
                        if (g.handle_command(src, dst)) {
                            display.apply_move(src, dst, g.isCastling);
                        }

                        srcSet = false;
                    } else {
                        store_mouse_pos(pos, src);

                        if (g.is_valid_pick(src)) srcSet = true;
                    }
                }
            }
        }
    }
}


/*
std::string strArr[7] = {" ", "P", "N", "B", "R", "Q", "K"};

void display_board(Game g) {
    string s;
    int piece;
    for (int i = 7; i > -1; i--) {
        s = "";
        for (int j = 0; j < 8; j++) {
            if (j != 0) s = s + "  ";
            piece = g.board[i][j];
            if (piece == EMPTY) s += "--";
            else if (piece > 0) {
                s += "W";
                s += strArr[abs(piece)];
            } else {
                s += "B";
                s += strArr[abs(piece)];
            }
        }
        std::cout << s << std::endl;
    }
}

void parse_command(Point *src, Point *dst) {
    char command[4];
    for (int i = 0; i < 4; i++) {
        std::cin >> command[i];
    }

    src->c = (int)(command[0] - 'a');
    src->r = (int)(command[1] - '1');
    dst->c = (int)(command[2] - 'a');
    dst->r = (int)(command[3] - '1');
}
*/