#pragma once
#include "PCH.hpp"
#include "Vector2D.hpp"

class Game;

class GameObject
{
public:
    virtual void Load(Vector2D position, double width, double height, std::string textureID);
    virtual void Update();
    virtual void Render();
    virtual void Clean();

    Vector2D GetPosition() const;
    double GetWidth() const;
    double GetHeight() const;
    SDL_Rect GetHitBox() const;

public:
    std::string m_ID;

protected:
    Game* m_game;
    Vector2D m_position;
    double m_width;
    double m_height;
    std::string m_textureID;
    SDL_Rect m_hitBox;
};