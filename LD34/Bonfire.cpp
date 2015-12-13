#include "PCH.hpp"
#include "Bonfire.hpp"

Bonfire::Bonfire(Game* game)
{
    m_game = game;
    m_ID = "bonfire";
}

void Bonfire::Update()
{
    GameObject::Update();
}

void Bonfire::Render()
{
    GameObject::Render();
}

void Bonfire::Clean()
{
    GameObject::Clean();
}