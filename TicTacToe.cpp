#include <iostream>
#include <vector>
#include <array>

using namespace std;
using namespace chrono;

#define vc vector<vector<char>>
const int n = 4;
vc board(n, vector<char>(n, '_'));

array<int,3> MIN_VALUE(int, int);
array<int, 3> MAX_VALUE(int, int);

void PrintBoardGame() {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
}

char IsEndGame() {
    for(int i = 0; i < n; ++i) {
        if (board[i][0] != '_' && board[i][0] == board[i][1] && board[i][1] == board[i][2] && board[i][2] == board[i][3])
            return board[i][0];
        if (board[0][i] != '_' && board[0][i] == board[1][i] && board[1][i] == board[2][i] && board[2][i] == board[3][i])
            return board[0][i];
    }
    if (board[0][0] != '_' && board[0][0] == board[1][1] && board[1][1] == board[2][2] && board[2][2] == board[3][3])
        return board[0][0];
    if (board[0][3] != '_' && board[0][3] == board[1][2] && board[1][2] == board[2][1] && board[2][1] == board[3][0])
        return board[0][3];

    for(int i = 0; i < n; ++i) {
        for(int j = 0; j < n; ++j) {
            if (board[i][j] == '_') return 'R';
        }
    }
    int cnt = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
           if (board[i][j] == '_') ++cnt;
        }
    }

    if (cnt == n * n) return '.';
    if (cnt == 0) return 'T';
    return 'T';
}


bool legalMove(int x, int y) {
    // if coordinate of x and y is legal and the cell x y is empty
    if (x < 0 || x > n || y < 0 || y > n || board[x][y] != '_') return false;
    // otherwise
    return true;
}


array<int, 3> MAX_VALUE(int alpha, int beta) {

    char winner = IsEndGame();
    if (winner == 'X') return {-1, 0, 0};
    if (winner == 'Y') return {1, 0, 0};
    if (winner == 'T') return {0, 0,0 };
    int val = -2;
    array <int, 3> coor{0, 0,0};
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (board[i][j] == '_') {
                board[i][j] = 'Y';
                array<int, 3> temp = MIN_VALUE(alpha, beta);
                if (temp[0] >= val) {
                    val = temp[0];
                    coor[0] = val;
                    coor[1] = i; coor[2] = j;
                }
                board[i][j] = '_';

                if (val >= beta) {
                    return {val, coor[1], coor[2]};
                }

                if (val > alpha) {
                    alpha = val;
                }
            }
        }
    }
    return coor;
}

array<int, 3> MIN_VALUE(int alpha, int beta) {
    char winner = IsEndGame();
    if (winner == 'X') return {-1, 0, 0};
    if (winner == 'Y') return {1, 0, 0};
    if (winner == 'T') return {0, 0,0 };
    int val = 2;
    array<int, 3> coor{0, -1, -1};
    for (int i = 0; i < n; ++i) {
        for (int j = 0;j < n; ++j) {
            if (board[i][j] == '_') {
                board[i][j] = 'X';
                array<int, 3> temp = MAX_VALUE(alpha, beta);
                if (temp[0] <= val) {
                    val = temp[0];
                    coor[0] = val;
                    coor[1] = i; coor[2] = j;
                }
                board[i][j] = '_';

                if (val <= alpha) {
                    return {val, coor[1], coor[2]};
                }
                if (val < beta) {
                    beta = val;
                }
            }
        }
    }
    return coor;
}



bool setCoordinate(int x, int y, const char& color) {
    if (legalMove(x, y)) {
        board[x][y] = color;
        return true;
    }
    return false;
}


void PlayTheGame() {
    cout << "Welcome to the game Tic-Tac_Toe " << endl;

    while (true) {
        PrintBoardGame();

        cout << "Enter your coordinate(x, y) separately: ";
        int x, y;
        cin >> x >> y;
        if (setCoordinate(x, y, 'X'));
        else {
            cout << "The move is not legal, try again!" << endl;
            continue;
        }
        char winner = IsEndGame();

        if (winner == 'X') {
            PrintBoardGame();
            cout << "The winner is X" << endl;
            break;
        }
        if (winner == 'Y') {
            PrintBoardGame();
            cout << "The winner is Y" << endl;

            break;
        }
        if (winner == 'T') {
            PrintBoardGame();
            cout << "Game is draw" << endl;
            break;
        }
        auto begin = std::chrono::system_clock::now();

        array<int, 3> playerY = MAX_VALUE(-2, 2);
        auto end = system_clock::now() - begin;
        auto timeInsecond = duration<double>(end).count();
        cout << "Take time: " << timeInsecond << " second" << endl;

        cout << "Value: " << playerY[0] << " Coordinate: (" << playerY[1] << " " << playerY[2] << ")" << endl;
        if (legalMove(playerY[1], playerY[2])) {
            board[playerY[1]][playerY[2]] = 'Y';
        }

        winner = IsEndGame();

        if (winner == 'X') {
            PrintBoardGame();
            cout << "The winner is X" << endl;
            break;
        }
        if (winner == 'Y') {
            PrintBoardGame();
            cout << "The winner is Y" << endl;
            break;
        }
        if (winner == 'T') {
            PrintBoardGame();
            cout << "Game is draw" << endl;
            break;
        }
    }

}


int main(void) {
    PlayTheGame();
    return 0;
}
