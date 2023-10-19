cls
rm .\Build\Rogue.exe
gcc main.c -o Build\Rogue -O1 -Wall -std=c99 -Wno-missing-braces -L ./lib/ -lraylib -lopengl32 -lgdi32 -lwinmm
.\Build\Rogue.exe