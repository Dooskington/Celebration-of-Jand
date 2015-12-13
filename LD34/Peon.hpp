#pragma once
#include "PCH.hpp"
#include "GameObject.hpp"
#include "Timer.hpp"
#include "Tree.hpp"
#include "Bonfire.hpp"

class Peon : public GameObject
{
public:
    Peon(Game* game, const Vector2D& position, const int& width, const int& height, const std::string& textureID);

    void Update();
    void Render();
    void Clean();

    void MoveTo(Vector2D dest);
    void Respawn();

    void IdleState();
    void WalkingState();
    void GatheringState();
    void SacrificeState();

public:
    Vector2D dest;

    bool m_isWandering = false;
    double walkSpeed = 32;
    double runSpeed = 64;
    double speedVariation = 0;

    enum State { IDLE, WALKING, GATHERING, SACRIFICE };
    State m_state;

    Bonfire* m_bonfire;
    GameObject* m_targetResource;
    std::string m_lastResource;

    int soundDelay;
    Timer m_gatherTimer;
    int waitTime;
    Timer m_idleTimer;
    int m_resources;

    double hopOffset;
    double hopIndex;
    double hopAmp = 3;
    double hopFreq;

private:
    typedef void (Peon::*stateFunction)();
    stateFunction m_stateHandler[5];
};