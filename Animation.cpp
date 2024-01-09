#include "Animation.h"
#include <iostream>
#include <cmath>

Animation::Animation()
{

}

Animation::Animation(const std::string& name, const sf::Texture& t)
    : Animation(name, t, 1, 0)
{

}

Animation::Animation(const std::string& name, const sf::Texture& t, size_t frameCount, size_t duration)
    : m_name        (name)
    , m_sprite      (t)
    , m_frameCount  (frameCount)
    , m_currentFrame(0)
    , m_duration    (duration)
{
    m_size = Vec2((float)t.getSize().x / frameCount, (float)t.getSize().y);
    m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
    m_sprite.setTextureRect(sf::IntRect(std::floor(m_currentFrame) * m_size.x, 0, m_size.x, m_size.y));
}

void Animation::update()
{
    m_currentFrame++;

    if (m_duration != 0)
    {   
        float animationFrame = (m_currentFrame / m_duration) % m_frameCount;

        float frameWidth = m_sprite.getTexture()->getSize().x / m_frameCount;
        float frameHeight = m_sprite.getTexture()->getSize().y;

        auto rectangle = sf::IntRect(animationFrame * frameWidth, 0, frameWidth, frameHeight);
        m_sprite.setTextureRect(rectangle);
    }
}

bool Animation::hasEnded() const
{
    if (m_duration != 0) { return (((m_currentFrame / m_duration) % m_frameCount) == (m_frameCount - 1)); }
    return false;
}

const std::string& Animation::getName() const
{
    return m_name;
}

const Vec2& Animation::getSize() const
{
    return m_size;
}

sf::Sprite& Animation::getSprite()
{
    return m_sprite;
}

