#include "Header.h"

int main() {
    // Initialize the window with given width, height, and title
    InitWindow(screenWidth, screenHeight, "N-Queens - Optimized Hill Climbing");
    SetTargetFPS(60);  // Set the frames per second for rendering
    srand(time(NULL)); // Seed the random number generator with the current time

    Estado current(N);  // Create the initial state with N queens
    int iterations = 0; // Counter for iterations of the algorithm
    int attempts = 1;   // Counter for how many times we've restarted
    bool finished = false; // Flag to check if a solution was found
    float timer = 0.0f;    // Timer to control the speed of updates

    // Main application loop
    while (!WindowShouldClose()) {
        timer += GetFrameTime(); // Accumulate elapsed time since last frame

        // If we haven't finished and the timer has reached the update threshold
        if (!finished && timer >= speed) {
            timer = 0.0f; // Reset the timer

            // Get the best neighboring state (optionally with randomness)
            Estado neighbor = current.MejorVecino(true);

            // If no better neighbor found (local maximum), restart with a new random state
            if (neighbor.conflictos >= current.conflictos) {
                current = Estado(N); // Create a new random state
                iterations = 0;
                attempts++;
            }
            else {
                current = neighbor; // Move to the better neighbor
                iterations++;

                // If a solution is found (no conflicts), mark as finished
                if (current.conflictos == 0)
                    finished = true;
            }
        }

        // Drawing section
        BeginDrawing();
        DrawTablero(N, current.reinas, iterations, current.conflictos, attempts, finished);
        EndDrawing();
    }

    // Close the application window and clean up
    CloseWindow();
    return 0;
}
