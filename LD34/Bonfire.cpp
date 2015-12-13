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
    //m_game->RenderTexture(m_textureID, m_position.GetX(), m_position.GetY(), m_width, m_height);
}

void Bonfire::Render()
{
    GameObject::Render();
}

void Bonfire::Clean()
{
    GameObject::Clean();
}