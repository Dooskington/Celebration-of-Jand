#pragma once
#include "PCH.hpp"
#include "GameObject.hpp"

class Bonfire : public GameObject
{
public:
    Bonfire(Game* game);

    void Update();
    void Render();
    void Clean();
};