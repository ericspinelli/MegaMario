#pragma once

#include <map>
#include <string>
#include "Animation.h"
#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>

class Assets
{
    std::map<std::string, sf::Texture>  m_textureMap;
    std::map<std::string, Animation>    m_animationMap;
    std::map<std::string, sf::Font>     m_fontMap;

    void addTexture(const std::string& textureName, const std::string& path, bool smooth = true);
    void addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t duration);
    void addFont(const std::string& fontName, const std::string& path);

public:
    Assets();

    void loadFromFile(const std::string& path);

    const sf::Texture&  getTexture(const std::string& textureName) const;
    const Animation&    getAnimation(const std::string& animationName) const;
    const sf::Font&     getFont(const std::string& fontName) const;
};