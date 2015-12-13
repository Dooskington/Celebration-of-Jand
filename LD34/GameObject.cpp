#include "PCH.hpp"
#include "GameObject.hpp"
#include "Game.hpp"

void GameObject::Load(Vector2D position, double width, double height, std::string textureID)
{
    m_position = position;
    m_width = width;
    m_height = height;
    m_textureID = textureID;
}

void GameObject::Update()
{
    m_hitBox.x = (int)m_position.GetX();
    m_hitBox.y = (int)m_position.GetY();
    m_hitBox.w = (int)m_width;
    m_hitBox.h = (int)m_height;
}

void GameObject::Render() 
{ 
    m_game->RenderTexture(m_textureID, m_position.GetX(), m_position.GetY(), m_width, m_height);
}

void GameObject::Clean()
{ 

}

Vector2D GameObject::GetPosition() const
{
    return m_position;
}

double GameObject::GetWidth() const
{
    return m_width;
}

double GameObject::GetHeight() const
{
    return m_height;
}

SDL_Rect GameObject::GetHitBox() const
{
    return m_hitBox;
}