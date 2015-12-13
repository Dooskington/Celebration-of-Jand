#include "PCH.hpp"
#include "Peon.hpp"
#include "Game.hpp"

Peon::Peon(Game* game) :
    m_state(IDLE),
    move(false),
    dest(0, 0),
    m_tree(nullptr)
{
    m_game = game;
    m_ID = "peon";

    m_stateHandler[IDLE] = &Peon::IdleState;
    m_stateHandler[WALKING] = &Peon::WalkingState;
    m_stateHandler[CHOPPING] = &Peon::ChoppingState;
}

void Peon::Update()
{
    GameObject::Update();

    stateFunction stateFunc = m_stateHandler[m_state];
    if (stateFunc != nullptr)
    {
       (this->*stateFunc)();
    }
}

void Peon::Render()
{
    GameObject::Render();
}

void Peon::Clean()
{
    GameObject::Clean();
}

void Peon::MoveTo(Vector2D dest)
{
    if (m_position != dest)
    {
        Vector2D start = m_position;
        double distance = Vector2D::Distance(start, dest);
        Vector2D direction = Vector2D::Normalize(dest - start);

        double speed;
        speed = runSpeed;

        m_position += direction * (speed * m_game->m_deltaTime);
        if (Vector2D::Distance(start, m_position) > distance)
        {
            m_position = dest;
        }
    }
}

void Peon::IdleState()
{
    // We are idle, find a tree to start chopping.
    if (m_tree == nullptr)
    {
        m_tree = m_game->FindTree(this);
    }
    else
    {
        // We have found a tree, now we need to begin walking to it.
        dest = m_tree->GetPosition();
        m_state = WALKING;
    }
}

void Peon::WalkingState()
{
    // Walk to our destination.
    MoveTo(dest);

    // If we have reached our destination, begin the next action
    if (m_position == dest)
    {
        if (!m_hasLogs)
        {
            m_state = CHOPPING;
        }
        else
        {
            m_game->PlaySound("drop");
            m_hasLogs = false;
            m_state = IDLE;
        }
    }
}

void Peon::ChoppingState()
{
    if (!m_chopTimer.IsStarted())
    {
        m_chopTimer.Start();
        soundDelay = rand() % 1000 + 700;
    }

    if (m_chopTimer.GetTime() > soundDelay)
    {
        m_chopTimer.Stop();
        m_game->PlaySound("chop");
        chopCount++;
    }

    if (chopCount >= 5)
    {
        // We have chopped the tree enough. Give this dude some logs.
        m_hasLogs = true;
        chopCount = 0;
    }

    if (m_hasLogs == true)
    {
        m_bonfire = m_game->FindBonfire(this);
        if (m_bonfire != nullptr)
        {
            dest = m_bonfire->GetPosition();
            m_state = WALKING;
        }
    }
}