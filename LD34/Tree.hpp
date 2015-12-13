#pragma once
#include "PCH.hpp"
#include "GameObject.hpp"

class Tree : public GameObject
{
public:
    Tree(Game* game);

    void Update();
    void Render();
    void Clean();
};