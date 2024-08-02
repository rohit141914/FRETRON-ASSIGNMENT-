#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>
#include <tuple>

using namespace std;

const int BOARD_SIZE = 10;
const char EMPTY = '.';
const char SOLDIER = 'S';
const char CASTLE = 'C';

struct Position {
    int x, y;

    bool operator<(const Position& other) const {
        return tie(x, y) < tie(other.x, other.y);
    }

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

struct PathStep {
    Position pos;
    string action;
};

void initializeBoard(vector<vector<char>>& board) {
    board.assign(BOARD_SIZE, vector<char>(BOARD_SIZE, EMPTY));
}

void placeSoldiers(vector<vector<char>>& board, const vector<Position>& soldiers) {
    for (const auto& soldier : soldiers) {
        board[soldier.x][soldier.y] = SOLDIER;
    }
}

void placeCastle(vector<vector<char>>& board, const Position& castlePos) {
    board[castlePos.x][castlePos.y] = CASTLE;
}

void printBoard(const vector<vector<char>>& board) {
    for (const auto& row : board) {
        for (const auto& cell : row) {
            cout << cell << ' ';
        }
        cout << endl;
    }
}

bool isInBounds(int x, int y) {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

vector<Position> getPossibleMoves(const Position& pos, const vector<vector<char>>& board) {
    vector<Position> moves;
    // Move right
    for (int y = pos.y + 1; y < BOARD_SIZE; ++y) {
        moves.push_back({pos.x, y});
        if (board[pos.x][y] == SOLDIER) break;
    }
    // Move down
    for (int x = pos.x + 1; x < BOARD_SIZE; ++x) {
        moves.push_back({x, pos.y});
        if (board[x][pos.y] == SOLDIER) break;
    }
    // Move left
    for (int y = pos.y - 1; y >= 0; --y) {
        moves.push_back({pos.x, y});
        if (board[pos.x][y] == SOLDIER) break;
    }
    // Move up
    for (int x = pos.x - 1; x >= 0; --x) {
        moves.push_back({x, pos.y});
        if (board[x][pos.y] == SOLDIER) break;
    }
    return moves;
}

void findPaths(Position startPos, const vector<vector<char>>& initialBoard, vector<PathStep>& path, set<vector<PathStep>>& uniquePaths) {
    vector<vector<char>> board = initialBoard;
    Position currentPos = startPos;

    while (true) {
        vector<Position> possibleMoves = getPossibleMoves(currentPos, board);
        if (possibleMoves.empty()) break;

        for (const auto& move : possibleMoves) {
            if (board[move.x][move.y] == SOLDIER) {
                path.push_back({move, "Kill (" + to_string(move.x + 1) + "," + to_string(move.y + 1) + "). Turn Left"});
                board[move.x][move.y] = EMPTY;
                currentPos = move;
                break;
            }
        }
    }
    if (currentPos == startPos) {
        uniquePaths.insert(path);
    }
}

int main() {
    int numSoldiers;
    cout << "find_my_home_castle -soldiers ";
    cin >> numSoldiers;

    vector<Position> soldiers(numSoldiers);
    for (int i = 0; i < numSoldiers; ++i) {
        cout << "Enter coordinates for soldier " << i + 1 << ": ";
        cin >> soldiers[i].x >> soldiers[i].y;
        soldiers[i].x--; soldiers[i].y--;
    }

    Position castlePos;
    cout << "Enter the coordinates for your \"special\" castle: ";
    cin >> castlePos.x >> castlePos.y;
    castlePos.x--; castlePos.y--;

    vector<vector<char>> board;
    initializeBoard(board);
    placeSoldiers(board, soldiers);
    placeCastle(board, castlePos);

    vector<PathStep> path;
    set<vector<PathStep>> uniquePaths;
    findPaths(castlePos, board, path, uniquePaths);

    cout << "Thanks. There are " << uniquePaths.size() << " unique paths for your 'special_castle'\n";
    for (const auto& uniquePath : uniquePaths) {
        cout << "Path:\nStart: (" << castlePos.x + 1 << "," << castlePos.y + 1 << ")\n";
        for (const auto& step : uniquePath) {
            cout << step.action << '\n';
        }
        cout << "Arrive (" << castlePos.x + 1 << "," << castlePos.y + 1 << ")\n";
    }

    return 0;
}