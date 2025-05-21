#include "raylib.h"
#include <vector>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <limits>

// Problem settings
const int N = 10;                  // Number of queens / board size
const int screenWidth = 600;
const int screenHeight = 600;
const int margin = 50;
const float speed = 0.1f;          // Speed of the search updates (in seconds)

// Structure representing a board state
struct State {
    std::vector<int> queens;      // queens[i] = row position of the queen in column i
    int conflicts;                // Number of conflicts in the current configuration

    // Constructor: initializes a random state
    State(int n) {
        queens.resize(n);
        for (int i = 0; i < n; ++i)
            queens[i] = GetRandomValue(0, n - 1); // Random row for each column
        conflicts = CalculateConflicts();
    }

    // Counts the number of conflicts between queens
    int CalculateConflicts() const {
        int count = 0;
        for (int i = 0; i < queens.size(); ++i) {
            for (int j = i + 1; j < queens.size(); ++j) {
                // Conflict if in same row or same diagonal
                if (queens[i] == queens[j] || abs(queens[i] - queens[j]) == abs(i - j))
                    count++;
            }
        }
        return count;
    }

    // Generates all possible neighboring states (by moving each queen to another row in its column)
    std::vector<State> GenerateNeighbors() const {
        std::vector<State> neighbors;
        for (int col = 0; col < queens.size(); ++col) {
            for (int row = 0; row < queens.size(); ++row) {
                if (queens[col] == row) continue; // Skip if already in that row
                State neighbor = *this;
                neighbor.queens[col] = row;       // Move queen to new row
                neighbor.conflicts = neighbor.CalculateConflicts();
                neighbors.push_back(neighbor);
            }
        }
        return neighbors;
    }

    // Returns the best neighbor (optionally, the second-best if the best equals the current)
    State BestNeighbor(bool useSecondBest = false) const {
        auto neighbors = GenerateNeighbors();
        std::sort(neighbors.begin(), neighbors.end(), [](const State& a, const State& b) {
            return a.conflicts < b.conflicts;
        });

        if (neighbors.empty()) return *this;

        if (useSecondBest && neighbors.size() > 1 && neighbors[0].conflicts == this->conflicts)
            return neighbors[1]; // Return second-best if the best is no improvement
        return neighbors[0];    // Return best neighbor
    }
};

// Draws the board, queens, and stats
void DrawBoard(int n, const std::vector<int>& queens, int iter, int conflicts, int attempts, bool finished) {
    float boardSize = screenWidth - 2 * margin;
    float cellSize = boardSize / n;

    ClearBackground(BLACK);

    // Draw chessboard
    for (int row = 0; row < n; ++row) {
        for (int col = 0; col < n; ++col) {
            bool isWhite = (row + col) % 2 == 0;
            DrawRectangle(
                margin + col * cellSize,
                margin + row * cellSize,
                cellSize,
                cellSize,
                isWhite ? WHITE : DARKGRAY
            );
        }
    }

    // Draw queens
    for (int col = 0; col < n; ++col) {
        int row = queens[col];
        DrawCircle(
            margin + col * cellSize + cellSize / 2,
            margin + row * cellSize + cellSize / 2,
            cellSize / 3,
            RED
        );
    }

    // Draw algorithm statistics
    DrawText(("Iterations: " + std::to_string(iter)).c_str(), margin, screenHeight - 50, 20, WHITE);
    DrawText(("Conflicts: " + std::to_string(conflicts)).c_str(), margin, screenHeight - 30, 20, WHITE);
    DrawText(("Attempts: " + std::to_string(attempts)).c_str(), margin * 5, screenHeight - 30, 20, WHITE);
    
    if (finished)
        DrawText("SOLUTION FOUND", screenWidth / 2 - 120, 20, 20, GREEN);
}
