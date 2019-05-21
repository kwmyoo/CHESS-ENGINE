#include "game.h"
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

int main() {
    // DISPLAY INITIAL SCREEN (select game type)
    // THEN GET TYPE OF EACH PLAYER
    // GET TIME LIMIT
    
    Game g(HUMAN, HUMAN);

    char command[4];
    Point src, dst;
    while (true) {
        display_board(g);
        for (int i = 0; i < 4; i++) {
            std::cin >> command[i];
        }
        src.c = (int)(command[0] - 'a');
        src.r = (int)(command[1] - '1');
        dst.c = (int)(command[2] - 'a');
        dst.r = (int)(command[3] - '1');

        g.handle_command(src, dst);
    }
}