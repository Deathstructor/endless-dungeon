#include "raylib.h"
#include "grid.c"

int main()
{
    InitWindow(1200, 800, "Rogue");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        
        EndDrawing();
    }
}