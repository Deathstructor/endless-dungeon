clear
rm Build/Rogue
cc main.c `pkg-config --libs --cflags raylib` -o Build/Rogue
./Build/Rogue