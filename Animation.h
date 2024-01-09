#pragma once

#include <string>
#include "Vec2.h"
#include <SFML/Graphics.hpp>

class Animation
{
    sf::Sprite  m_sprite;
    size_t      m_frameCount    = 1; // total number of frames of animation
    size_t      m_currentFrame  = 0; // current frame of animation being played
    size_t      m_duration      = 0; // game frame duration of each animation frame
    Vec2        m_size          = {1, 1}; //size of the animation frame
    std::string m_name = "none";     // animation name

public:
    Animation();
    Animation(const std::string& name, const sf::Texture& t);
    Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t duration);

    void update();
    bool hasEnded() const;
    const std::string& getName() const;
    const Vec2& getSize() const;
    sf::Sprite& getSprite();
};