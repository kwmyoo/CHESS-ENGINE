#include <iostream>
using namespace std;

enum P_type { HUMAN, ENGINE }; // player type
enum Side { WHITE, BLACK };

typedef struct point {
    int r, c;
};

class Game {
    public:
    
    int board[8][8];
    int wp; // white player
    int bp; // black player
    int curr_turn;

    long white_timer;
    long black_timer;

    Game(p_type wType, p_type bType) {
        wp = wType;
        bp = bType;

        curr_turn = WHITE;
    }

    bool is_valid_move(point src, point dst) {

    }

}

void main() {
    // DISPLAY INITIAL SCREEN (select game type)
    // THEN GET TYPE OF EACH PLAYER
    // GET TIME LIMIT
    
    // start game
    Game g(white_type, black_type);


}