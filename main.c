#include <stdlib.h>
#include <math.h>
#include "raylib.h"

typedef struct square
{
    struct square *rec1;
    struct square *rec2;
    Rectangle info;
} square;

const int max_partitions = 4;
int leaves = 0;

square *SquareCreate(Rectangle value)
{
    square *new_square = (square *)malloc(sizeof(square));
    if (new_square != NULL)
    {
        new_square->rec1 = NULL;
        new_square->rec2 = NULL;
        new_square->info = value;
    }

    return new_square;
}

void PartitionCreate(short depth, square *leaf, square* save_leaf)
{
    int rdm_axis = GetRandomValue(0, 1);

    if (depth > max_partitions)
    {
        DrawRectangleRec(leaf->info, RED);
        DrawRectangleLinesEx(leaf->info, 1, WHITE);

        save_leaf += leaves;
        save_leaf = leaf;
        leaves++;
        return;
    }

    Rectangle child_1, child_2;

    if (rdm_axis)
    {
        child_1 = (Rectangle){leaf->info.x, leaf->info.y, leaf->info.width / 2, leaf->info.height};
        child_2 = (Rectangle){leaf->info.x + leaf->info.width / 2, leaf->info.y, leaf->info.width / 2, leaf->info.height};
    }
    else
    {
        child_1 = (Rectangle){leaf->info.x, leaf->info.y, leaf->info.width, leaf->info.height / 2};
        child_2 = (Rectangle){leaf->info.x, leaf->info.y + leaf->info.height / 2, leaf->info.width, leaf->info.height / 2};
    }

    leaf->rec1 = SquareCreate(child_1);
    leaf->rec2 = SquareCreate(child_2);

    PartitionCreate(depth + 1, leaf->rec1, save_leaf);
    PartitionCreate(depth + 1, leaf->rec2, save_leaf);
}

int main()
{
    InitWindow(1200, 800, "Rogue");

    square *save_leaf[(int)pow(2, max_partitions + 1)];

    Rectangle root = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};
    PartitionCreate(0, SquareCreate(root), save_leaf);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);


        EndDrawing();
    }
}