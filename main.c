#include <stdlib.h>
#include <math.h>
#include "raylib.h"
#include "raymath.h"

typedef struct square
{
    struct square *rec1;
    struct square *rec2;
    Rectangle info;
} square;

const int max_partitions = 3;
Rectangle *rooms_arr;
square *leaf_arr;
int index = 0;

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

void PartitionCreate(short depth, square *leaf)
{
    int rdm_axis = GetRandomValue(0, 1);

    if (depth > max_partitions)
    {
        int random_width = GetRandomValue(40, 100);
        int random_height = GetRandomValue(40, 100);

        leaf_arr[index] = *leaf;
        
        if(random_width < leaf->info.width && random_height < leaf->info.height)
        {
            Rectangle room = (Rectangle){GetRandomValue(leaf->info.x, leaf->info.x + leaf->info.width - random_width),
                                        GetRandomValue(leaf->info.y, leaf->info.y + leaf->info.height - random_height),
                                        random_width,
                                        random_height
                                        };

            rooms_arr[index] = room;
        }


        index++;
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

    PartitionCreate(depth + 1, leaf->rec1);
    PartitionCreate(depth + 1, leaf->rec2);
}

int main()
{
    InitWindow(1200, 800, "Rogue");
    SetTargetFPS(60);

    Rectangle rooms[(int)pow(2, max_partitions + 1)];
    rooms_arr = rooms;

    square save_leaf[(int)pow(2, max_partitions + 1)];
    leaf_arr = save_leaf;

    Rectangle root = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};
    PartitionCreate(0, SquareCreate(root));

    // short random_room_width[(int)pow(2, max_partitions + 1)];
    // short random_room_height[(int)pow(2, max_partitions + 1)];
    // short random_room_x_offset[(int)pow(2, max_partitions + 1)];
    // short random_room_y_offset[(int)pow(2, max_partitions + 1)];

    // for (int i = 0; i < (int)pow(2, max_partitions + 1); i++)
    // {
    //     random_room_width[i] = GetRandomValue(60, 100);
    //     random_room_height[i] = GetRandomValue(60, 100);
    //     random_room_x_offset[i] = GetRandomValue(save_leaf[i].info.x, save_leaf[i].info.width - random_room_width[i]);
    //     random_room_y_offset[i] = GetRandomValue(save_leaf[i].info.y, save_leaf[i].info.height - random_room_height[i]);
    // }

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < (int)pow(2, max_partitions + 1); i++)
        {
            DrawRectangleRec(save_leaf[i].info, BLACK);
            DrawRectangleLinesEx(save_leaf[i].info, 1, WHITE);

            DrawRectangleRec(rooms[i], BLUE);

            // DrawRectangle(save_leaf[i].info.x + random_room_x_offset[i],
            //               save_leaf[i].info.y + random_room_y_offset[i],
            //               random_room_width[i],
            //               random_room_height[i],
            //               BLUE);
        }

        EndDrawing();
    }

    // for (int i = 0; i < (int)pow(2, max_partitions + 1); i++)
    // {
    //     free(&save_leaf[i]);
    // }
}