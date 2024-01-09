#include "GameEngine.h"

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::init(const std::string path)
{
    // Load all assets once and access from various Scenes
    m_assets.loadFromFile(path);

    // Configure main SFML window which is shared by all scenes
    m_window.create(sf::VideoMode(1280, 720), "Mega Mario");
    m_window.setFramerateLimit(60);

    // Load initial Scene
    changeScene("MENU", std::make_shared<Scene_Menu>(this));
}

void GameEngine::update()
{
    sUserInput();
    m_sceneMap.at(m_currentScene)->update();
}

// Handle raw input from users only.  Input mapping and logic is handled by Scene class
void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        // Enable [X] in window top-right to close program
        if (event.type == sf::Event::Closed) {quit();}
        
        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // If the current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end()) {continue;}

            // Determine start or end action by whether it was key press or release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // Look up action and send action to the scene to be executed
            currentScene()->sDoAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap.at(m_currentScene);
}

void GameEngine::changeScene(const std::string& sceneName, std::shared_ptr<Scene> scene, bool endCurrentScene)
{ 
    /*
    if (!(m_sceneMap.find(sceneName) == m_sceneMap.end()))
    {
        std::cout << "not found" << std::endl;
        m_sceneMap[sceneName] = scene;
    }
    */

    m_sceneMap[sceneName] = scene;
    m_currentScene = sceneName;
}

void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

sf::RenderWindow& GameEngine::window()
{
    return m_window;
}

const Assets& GameEngine::assets() const
{
    return m_assets;
}

bool GameEngine::isRunning()
{
    return m_running & m_window.isOpen();
}
