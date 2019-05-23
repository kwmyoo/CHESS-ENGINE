#include "game.h"
#include "display.h"
#include <iostream>
#include <string>

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

int main() {
    // DISPLAY INITIAL SCREEN (select game type)
    // THEN GET TYPE OF EACH PLAYER
    // GET TIME LIMIT
    
    Game g(HUMAN, HUMAN);
    DisplayGame display;

    Point *src = new Point, *dst = new Point;
    bool srcSet = false;
    sf::Vector2i srcPos, dstPos;
    int r, c;

    sf::Texture board;
    board.loadFromFile("images/board.png");
    const Sprite boardSprite(board);

    while (display.window->isOpen()) {
        display.draw_game(&boardSprite);
        sf::Event e;

        while (display.window->pollEvent(e)) {
            if (e.type == sf::Event::Closed) display.window->close();

            if (e.type == sf::Event::MouseButtonReleased) {
                if (e.mouseButton.button == sf::Mouse::Left) {
                    if (srcSet) {
                        dstPos = sf::Mouse::getPosition(*(display.window));
                        c = dstPos.x / display.size;
                        r = 7 - (dstPos.y / display.size);

                        dst->r = r;
                        dst->c = c;

                        if (g.handle_command(src, dst)) {
                            display.apply_move(src, dst);

                            srcSet = false;
                        }
                    } else {
                        srcPos = sf::Mouse::getPosition(*(display.window));
                        c = dstPos.x / display.size;
                        r = 7 - (dstPos.y / display.size);

                        src->r = r;
                        src->c = c;

                        if (g.is_valid_pick(src)) srcSet = true;
                    }
                }
            }
        }
    }
}