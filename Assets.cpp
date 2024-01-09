#include "Assets.h"

Assets::Assets() {}

void Assets::loadFromFile(const std::string& path)
{
    std::ifstream fin(path);
    std::string temp;

    while (fin >> temp)
    {
        if (temp == "Texture")
        {
            std::string name, path;
            fin >> name >> path;
            addTexture(name, path);
        }
        else if (temp == "Animation")
        {
            std::string name, texture;
            size_t frames, duration;
            fin >> name >> texture >> frames >> duration;
            addAnimation(name, texture, frames, duration);
        }
        else if (temp == "Font")
        {
            std::cout << "font font font" << std::endl;
            std::string name, path;
            fin >> name >> path;
            addFont(name, path);
        }
    }
}

void Assets::addTexture(const std::string& textureName, const std::string& path, bool smooth)
{
    m_textureMap[textureName] = sf::Texture();

    if (!m_textureMap[textureName].loadFromFile(path))
    {
        std::cerr << "Could not load texture file: " << path << std::endl;
        m_textureMap.erase(textureName);
    }
    else
    {
        std::cout << "Loaded: " << textureName << std::endl;
        m_textureMap[textureName].setSmooth(smooth);
    }
}

void Assets::addAnimation(const std::string& animationName, const std::string& textureName, size_t frameCount, size_t duration)
{
    m_animationMap[animationName] = Animation(animationName, getTexture(textureName), frameCount, duration);
    std::cout << "Added: " << animationName << std::endl;
}

void Assets::addFont(const std::string& fontName, const std::string& path)
{
    m_fontMap[fontName] = sf::Font();

    if (!m_fontMap[fontName].loadFromFile(path))
    {
        std::cerr << "Could not load font file: " << path << std::endl;
        m_fontMap.erase(fontName);
    }
    else
    {
        std::cout << "Loaded: " << fontName << std::endl;
    }
}

const sf::Texture&  Assets::getTexture(const std::string& textureName) const
{
    return m_textureMap.at(textureName);
}

const Animation&    Assets::getAnimation(const std::string& animationName) const
{
    return m_animationMap.at(animationName);
}

const sf::Font&     Assets::getFont(const std::string& fontName) const
{
    return m_fontMap.at(fontName);
}

/*
int main()
{
    std::cout << "loading assets.txt" << std::endl;
    Assets m_assets;
    m_assets.loadFromFile("bin/assets.txt");
    
    std::cout << "getting texture" << std::endl;
    std::cout << m_assets.getTexture("TexBrick").getSize().x << std::endl;

    std::cout << "getting animation" << std::endl;
    std::cout << m_assets.getAnimation("Run").getName() << std::endl;

    std::cout << "getting font" << std::endl;
    std::cout << m_assets.getFont("Megaman").getInfo().family << std::endl;

    std::cout << "success" << std::endl;
}
*/