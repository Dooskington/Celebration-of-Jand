#include "PCH.hpp"
#include "Game.hpp"

int main(int argc, char** argv)
{
    std::srand(std::time(0));

    Game game;
    game.Start();

    return 0;
}