#pragma once
#include "PCH.hpp"
#include "GameObject.hpp"
#include "Timer.hpp"
#include "Tree.hpp"
#include "Bonfire.hpp"

class Peon : public GameObject
{
public:
    Peon(Game* game);

    void Update();
    void Render();
    void Clean();

    void MoveTo(Vector2D dest);

    void IdleState();
    void WalkingState();
    void ChoppingState();

public:
    bool move;
    Vector2D dest;

    double walkSpeed = 32;
    double runSpeed = 64;

    enum State { IDLE, WALKING, CHOPPING };
    State m_state;

    Bonfire* m_bonfire;
    Tree* m_tree;

    int soundDelay;
    Timer m_chopTimer;
    int chopCount;
    bool m_hasLogs = false;

private:
    typedef void (Peon::*stateFunction)();
    stateFunction m_stateHandler[3];
};