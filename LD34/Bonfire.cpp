#include "PCH.hpp"
#include "Bonfire.hpp"
#include "Game.hpp"

Bonfire::Bonfire(Game* game)
{
    m_game = game;
    m_ID = "bonfire";
}

void Bonfire::Update()
{
    GameObject::Update();
    
    int resources = m_game->GetResources();
    if (resources <= 50)
    {
        m_textureID = "bonfire_0";
    }
    else if (resources <= 400)
    {
        m_textureID = "bonfire_1";
    }
    else if (resources <= 1000)
    {
        m_textureID = "bonfire_2";
    }
    else if (resources <= 2000)
    {
        m_textureID = "bonfire_3";
    }
    else if (resources > 2000)
    {
        m_textureID = "bonfire_4";
    }
    else
    {
        m_textureID = "bonfire_0";
    }
}

void Bonfire::Render()
{
    GameObject::Render();
}

void Bonfire::Clean()
{
    GameObject::Clean();
}