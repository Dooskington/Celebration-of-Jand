#pragma once
#include "PCH.hpp"
#include "GameObject.hpp"

class Stone : public GameObject
{
public:
    Stone(Game* game);

    void Update();
    void Render();
    void Clean();
};