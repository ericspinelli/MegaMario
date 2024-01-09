#include "Scene_Menu.h"

Scene_Menu::Scene_Menu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void Scene_Menu::init()
{    
    // set up menu strings and text options
    m_title = "Mega Mario";

    m_menuStrings.push_back("Level  1");
    m_menuStrings.push_back("Level  2");
    m_menuStrings.push_back("Level  3");
    
    m_menuText.setFont(m_game->assets().getFont("Megaman"));
    m_menuText.setCharacterSize(64);
    
    // set path to level config files
    m_levelPaths.push_back("bin/level1.txt");
    m_levelPaths.push_back("bin/level2.txt");
    m_levelPaths.push_back("bin/level3.txt");

    // bind keys for navigating menu
    registerAction(sf::Keyboard::Escape,"QUIT");
    registerAction(sf::Keyboard::Up,    "UP");      // move up in menu (looping)
    registerAction(sf::Keyboard::Down,  "DOWN");    // move down in menu (looping)
    registerAction(sf::Keyboard::Enter, "PLAY");    // select level and play
}

void Scene_Menu::registerAction(int inputKey, const std::string& actionName)
{
    m_actionMap[inputKey] = actionName;
}

void Scene_Menu::update()
{
    m_entityManager.update();
    sRender();
}

void Scene_Menu::onEnd()
{
    m_game->quit();
}

void Scene_Menu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "UP")
        {
            if (m_selectedMenuIndex > 0) {m_selectedMenuIndex--;}
            else {m_selectedMenuIndex = m_menuStrings.size() - 1;}
        }
        else if (action.name() == "DOWN")
        {
            m_selectedMenuIndex = (m_selectedMenuIndex + 1) % m_menuStrings.size();
        }
        else if (action.name() == "PLAY")
        {
            std::cout << "PLAY! " << m_levelPaths[m_selectedMenuIndex] << std::endl;

            m_game->changeScene("PLAY", std::make_shared<Scene_Play>(m_game, m_levelPaths[m_selectedMenuIndex]));
        }
        else if (action.name() == "QUIT")
        {
            onEnd();
        }
    }
}

void Scene_Menu::sRender()
{
    // clear the window to a blue
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(100, 100, 255));

    // draw the game title in the top-left of the screen
    m_menuText.setCharacterSize(48);
    m_menuText.setString(m_title);
    m_menuText.setFillColor(sf::Color::Black);
    m_menuText.setPosition(sf::Vector2f(10, 10));
    m_game->window().draw(m_menuText);

    // draw all of the menu options
    for (size_t i = 0; i < m_menuStrings.size(); i++)
    {
        m_menuText.setString(m_menuStrings[i]);
        m_menuText.setFillColor(i == m_selectedMenuIndex ? sf::Color::White : sf::Color(0, 0, 0));
        m_menuText.setPosition(sf::Vector2f(10, 110 + i * 72));
        m_game->window().draw(m_menuText);
    }

    m_game->window().display();
}