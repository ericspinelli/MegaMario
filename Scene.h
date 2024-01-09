#pragma once

#include "Action.h"
#include "EntityManager.h"
#include <memory>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{
protected:
    GameEngine*     m_game = nullptr;
    EntityManager   m_entityManager;
    ActionMap       m_actionMap;
    bool            m_paused = false;
    bool            m_hasEnded = false;
    size_t          m_currentFrame = 0;

    virtual void onEnd() = 0;
    void setPaused(bool paused) {m_paused = paused;}

public:
    Scene();
    Scene(GameEngine* gameEngine) : m_game(gameEngine) {}

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    //virtual void doAction(const Action& action);
    void simulate(const size_t frames);
    //void registerAction(int inputKey, const std::string& actionName);
    ActionMap& getActionMap() {return m_actionMap;}

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;
};