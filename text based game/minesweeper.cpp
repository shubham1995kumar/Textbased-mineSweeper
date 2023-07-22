#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iomanip>

using namespace std;

class Cell {
private:
    bool mine;
    bool opened;
    bool flagged;
    int adjacentMines;

public:
    Cell() : mine(false), opened(false), flagged(false), adjacentMines(0) {}

    bool isMine() const {
        return mine;
    }

    void setMine() {
        mine = true;
    }

    bool isOpened() const {
        return opened;
    }

    void setOpen() {
        opened = true;
    }

    bool isFlagged() const {
        return flagged;
    }

    void toggleFlag() {
        flagged = !flagged;
    }

    int getAdjacentMines() const {
        return adjacentMines;
    }

    void incrementAdjacentMines() {
        adjacentMines++;
    }
};

class Minesweeper {
private:
    int boardSize;
    vector<vector<Cell>> board;
    int minesCount;
    int openedCellsCount;

public:
    Minesweeper(int size, int mines) : boardSize(size), minesCount(mines), openedCellsCount(0) {
        initializeBoard();
        placeMines();
        calculateAdjacentMines();
    }

    void play() {
        printWelcomeMessage();
        while (true) {
            printBoard();

            int x, y;
            cout << "Enter the coordinates (x, y): ";
            cin >> x >> y;

            if (!isValidCoordinate(x, y)) {
                cout << "Invalid coordinate. Try again." << endl;
                continue;
            }

            if (board[x][y].isMine()) {
                cout << "*****************************" << endl;
                cout << "* Boom! You hit a mine. Game Over! *" << endl;
                cout << "*****************************" << endl;
                revealBoard();
                break;
            }

            openCell(x, y);

            if (isGameWon()) {
                cout << "*******************************" << endl;
                cout << "* Congratulations! You won the game! *" << endl;
                cout << "*******************************" << endl;
                revealBoard();
                break;
            }
        }
    }

private:
    void initializeBoard() {
        board.resize(boardSize, vector<Cell>(boardSize));
    }

    void placeMines() {
        srand(time(nullptr));
        int minesToPlace = minesCount;

        while (minesToPlace > 0) {
            int x = rand() % boardSize;
            int y = rand() % boardSize;

            if (!board[x][y].isMine()) {
                board[x][y].setMine();
                minesToPlace--;
            }
        }
    }

    void calculateAdjacentMines() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                if (board[i][j].isMine()) {
                    updateAdjacentCells(i, j);
                }
            }
        }
    }

    void updateAdjacentCells(int x, int y) {
        const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
        const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

        for (int i = 0; i < 8; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            if (isValidCoordinate(newX, newY)) {
                board[newX][newY].incrementAdjacentMines();
            }
        }
    }

    bool isValidCoordinate(int x, int y) const {
        return x >= 0 && x < boardSize && y >= 0 && y < boardSize;
    }

    void openCell(int x, int y) {
        Cell& cell = board[x][y];

        if (!cell.isOpened() && !cell.isFlagged()) {
            cell.setOpen();
            openedCellsCount++;

            if (cell.getAdjacentMines() == 0) {
                const int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
                const int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

                for (int i = 0; i < 8; i++) {
                    int newX = x + dx[i];
                    int newY = y + dy[i];

                    if (isValidCoordinate(newX, newY)) {
                        openCell(newX, newY);
                    }
                }
            }
        }
    }

    void revealBoard() {
        for (int i = 0; i < boardSize; i++) {
            for (int j = 0; j < boardSize; j++) {
                board[i][j].setOpen();
            }
        }

        printBoard();
    }

    bool isGameWon() const {
        int totalCells = boardSize * boardSize;
        int nonMineCells = totalCells - minesCount;
        return openedCellsCount == nonMineCells;
    }

    void printWelcomeMessage() const {
        cout << "**********************************" << endl;
        cout << "* Welcome to Minesweeper!        *" << endl;
        cout << "* Uncover all the cells that     *" << endl;
        cout << "* don't contain mines to win     *" << endl;
        cout << "* the game.                      *" << endl;
        cout << "* Enter the coordinates (x, y)   *" << endl;
        cout << "* to open a cell.                *" << endl;
        cout << "* Be careful not to hit a mine!  *" << endl;
        cout << "* Good luck!                     *" << endl;
        cout << "**********************************" << endl;
        cout << endl;
    }

    void printBoard() const {
        cout << "   ";
        for (int i = 0; i < boardSize; i++) {
            cout << setw(2) << i << " ";
        }
        cout << endl;

        cout << "  +";
        for (int i = 0; i < boardSize; i++) {
            cout << "--+";
        }
        cout << endl;

        for (int i = 0; i < boardSize; i++) {
            cout << setw(2) << i << "|";
            for (int j = 0; j < boardSize; j++) {
                const Cell& cell = board[i][j];

                if (cell.isFlagged()) {
                    cout << " F|";
                } else if (!cell.isOpened()) {
                    cout << " .|";
                } else if (cell.isMine()) {
                    cout << " *|";
                } else {
                    int adjacentMines = cell.getAdjacentMines();
                    cout << setw(2) << adjacentMines << "|";
                }
            }
            cout << endl;

            cout << "  +";
            for (int j = 0; j < boardSize; j++) {
                cout << "--+";
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main() {
    int boardSize, minesCount;

    cout << "Enter the size of the board (n x n, n >= 9): ";
    cin >> boardSize;

    cout << "Enter the number of mines: ";
    cin >> minesCount;

    Minesweeper game(boardSize, minesCount);
    game.play();

    return 0;
}
