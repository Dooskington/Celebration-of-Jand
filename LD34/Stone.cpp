#include "PCH.hpp"
#include "Stone.hpp"
#include "Game.hpp"

Stone::Stone(Game* game)
{
    m_game = game;
    m_ID = "stone";
}

void Stone::Update()
{
    GameObject::Update();
}

void Stone::Render()
{
    GameObject::Render();
}

void Stone::Clean()
{
    GameObject::Clean();
}