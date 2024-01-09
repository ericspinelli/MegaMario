#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <deque>

#include "EntityManager.h"
#include "GameEngine.h"
#include "Scene_Play.h"

class Scene_Menu : public Scene
{
protected:
    std::string                 m_title;
    std::vector<std::string>    m_menuStrings;
    std::vector<std::string>    m_levelPaths;
    sf::Text                    m_menuText;
    size_t                      m_selectedMenuIndex = 0;

    void init();
    void registerAction(int inputKey, const std::string& actionName);

    void update();
    void onEnd();

    void sDoAction(const Action& action);
    void sRender();

public:
    Scene_Menu(GameEngine* gameEngine = nullptr);
    //void registerAction(int inputKey, const std::string& actionName);
    //void sRender();
};