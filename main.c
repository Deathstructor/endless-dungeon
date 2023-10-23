#include <stdio.h>
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

typedef struct line_point
{
    Vector2 start;
    Vector2 end;
} line_point;

typedef struct player
{
    Vector2 pos;
    Color col;
    int size;
    bool exist;
} player;

const int max_partitions = 3;
Rectangle *rooms_arr;
line_point *lp_arr;
Vector2 lp_curr = (Vector2){-1, -1};
Vector2 lp_prev = (Vector2){-1, -1};
square *leaf_arr;
int arr_index = 0;

player p;
void PlayerDraw()
{
    if (!p.exist)
    {
        int rdm_spawn_room = GetRandomValue(0, (int)pow(2, max_partitions + 1));
        Vector2 rdm_spawn_pos = (Vector2){GetRandomValue(rooms_arr[rdm_spawn_room].x,
                                                         rooms_arr[rdm_spawn_room].x + rooms_arr[rdm_spawn_room].width - p.size),
                                          GetRandomValue(rooms_arr[rdm_spawn_room].y,
                                                         rooms_arr[rdm_spawn_room].y + rooms_arr[rdm_spawn_room].height - p.size)};

        p.pos = rdm_spawn_pos;
        p.size = 10;
        p.col = RED;

        if (rdm_spawn_pos.x >= 10 && rdm_spawn_pos.x <= GetScreenWidth() && rdm_spawn_pos.y >= 0 && rdm_spawn_pos.y <= GetScreenHeight())
            p.exist = true;
    }
    else
    {
        if (IsKeyDown(KEY_A))
            p.pos.x--;
        if (IsKeyDown(KEY_D))
            p.pos.x++;
        if (IsKeyDown(KEY_W))
            p.pos.y--;
        if (IsKeyDown(KEY_S))
            p.pos.y++;

        DrawRectangle(p.pos.x, p.pos.y, p.size, p.size, p.col);
        printf("X: %f, Y: %f\n", p.pos.x, p.pos.y);
    }
}

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
        int random_width = GetRandomValue(80, 120);
        int random_height = GetRandomValue(80, 120);

        if (random_width < leaf->info.width && random_height < leaf->info.height)
        {
            Rectangle room = (Rectangle){GetRandomValue(leaf->info.x, leaf->info.x + leaf->info.width - random_width),
                                         GetRandomValue(leaf->info.y, leaf->info.y + leaf->info.height - random_height),
                                         random_width,
                                         random_height};

            if (lp_curr.x == -1 && lp_curr.y == -1)
            {
                lp_curr = (Vector2){room.x + room.width / 2, room.y + room.height / 2};
            }
            else
            {
                lp_prev = lp_curr;
                lp_curr = (Vector2){room.x + room.width / 2, room.y + room.height / 2};
                lp_arr[arr_index - 1] = (line_point){lp_prev, lp_curr};
            }

            rooms_arr[arr_index] = room;
        }

        leaf_arr[arr_index] = *leaf;

        arr_index++;
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

    line_point line_points[(int)pow(2, max_partitions + 1) - 1];
    lp_arr = line_points;

    square save_leaf[(int)pow(2, max_partitions + 1)];
    leaf_arr = save_leaf;

    Rectangle root = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};
    PartitionCreate(0, SquareCreate(root));

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        for (int i = 0; i < (int)pow(2, max_partitions + 1) - 1; i++)
        {
            DrawLineEx(
                (Vector2){
                    lp_arr[i].start.x,
                    lp_arr[i].start.y},
                (Vector2){
                    (lp_arr[i].start.x < lp_arr[i].end.x ? 7.5 : -7.5) + lp_arr[i].end.x,
                    lp_arr[i].start.y},
                15,
                DARKBLUE);

            DrawLineEx(
                (Vector2){
                    lp_arr[i].end.x,
                    (lp_arr[i].start.y < lp_arr[i].end.y ? 7.5 : -7.5) + lp_arr[i].start.y},
                (Vector2){
                    lp_arr[i].end.x,
                    (lp_arr[i].start.y < lp_arr[i].end.y ? 7.5 : -7.5) + lp_arr[i].end.y},
                15,
                DARKBLUE);
        }

        for (int i = 0; i < (int)pow(2, max_partitions + 1); i++)
        {

            // DrawRectangleRec(save_leaf[i].info, BLACK);
            // DrawRectangleLinesEx(save_leaf[i].info, 1, WHITE);

            DrawRectangleRec(rooms[i], BLUE);
        }

        PlayerDraw();

        EndDrawing();
    }

    // for (int i = 0; i < (int)pow(2, max_partitions + 1); i++)
    // {
    //     free(&save_leaf[i]);
    // }
}