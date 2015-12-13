#include "PCH.hpp"
#include "Tree.hpp"
#include "Game.hpp"

Tree::Tree(Game* game)
{
    m_game = game;
    m_ID = "tree";
}

void Tree::Update()
{
    GameObject::Update();
}

void Tree::Render()
{
    GameObject::Render();
}

void Tree::Clean()
{
    GameObject::Clean();
}