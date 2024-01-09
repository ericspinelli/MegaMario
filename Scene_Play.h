#pragma once

#include "Scene.h"
#include <map>
#include <memory>
#include <SFML/Graphics.hpp>
#include "Action.h"

#include "EntityManager.h"
#include "GameEngine.h"
#include "Physics.h"

class Scene_Play : public Scene
{
    struct PlayerConfig
    {
        float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, MAXJUMP, GRAVITY;
        std::string CHARACTER, WEAPON;
    };

    struct WeaponConfig
    {
        float SPEED, LIFESPAN;
        std::string WEAPON;
    };

protected:
    std::shared_ptr<Entity> m_player;
    std::string             m_levelPath;
    PlayerConfig            m_playerConfig;
    WeaponConfig            m_weaponConfig;
    bool                    m_drawTextures = true;
    bool                    m_drawCollision = false;
    bool                    m_drawGrid = false;
    const Vec2              m_gridSize = {64, 64};
    sf::Text                m_gridText;


    void init(const std::string& levelPath);
    void registerAction(sf::Keyboard::Key input, std::string actionName);
    
    void loadLevel(const std::string& filename);
    Vec2 gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity);

    void spawnPlayer();
    void spawnBullet();

    void update();
    void sDoAction(const Action& action);
    void sMovement();
    void sCollision();
    void sLifespan();
    void sAnimation();
    void sRender();

    void onEnd();

public:
    Scene_Play(GameEngine* gameEngine, const std::string& levelPath);

};