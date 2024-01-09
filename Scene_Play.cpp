#include "Scene_Play.h"

Scene_Play::Scene_Play(GameEngine* gameEngine, const std::string& levelPath)
    : Scene(gameEngine)
    , m_levelPath(levelPath)
{
    init(m_levelPath);
}

void Scene_Play::init(const std::string& levelPath)
{
    // Bind keys for game play and debugging options
    registerAction(sf::Keyboard::P,     "PAUSE");
    registerAction(sf::Keyboard::Escape,"QUIT");
    registerAction(sf::Keyboard::T,     "TOGGLE_TEXTURE");      // toggle drawing (T)extures
    registerAction(sf::Keyboard::C,     "TOGGLE_COLLISION");    // toggle drawing (C)ollision Boxes
    registerAction(sf::Keyboard::G,     "TOGGLE_GRID");         // toggle drawing (G)rid

    // Bind keys for player movement and actions
    registerAction(sf::Keyboard::Left,  "LEFT");                // player moves LEFT
    registerAction(sf::Keyboard::Right, "RIGHT");               // player moves RIGHT
    registerAction(sf::Keyboard::Space, "JUMP");                // player JUMPs
    registerAction(sf::Keyboard::Up,    "JUMP");                // player JUMPs
    registerAction(sf::Keyboard::A,     "SHOOT");               // player SHOOTs

    // Init text for debugging grid
    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Arial"));

    // Load level from the level file
    loadLevel(levelPath);
}

void Scene_Play::registerAction(sf::Keyboard::Key input, std::string actionName)
{
    m_actionMap[input] = actionName;
}

void Scene_Play::loadLevel(const std::string& filename)
{
    // reset the entity manager whenever level is loaded
    m_entityManager = EntityManager();

    std::ifstream fin(filename);
    std::string temp;

    while (fin >> temp)
    {
        if (temp == "Tile")
        {
            
            std::string animName;
            float gridX, gridY;
            fin >> animName >> gridX >> gridY;                      // Animation name, grid position (x,y) = (64 x 64px)

            // create tile entity WITH bounding box
            auto tile = m_entityManager.addEntity("tile");      

            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animName), true);          
            tile->addComponent<CBoundingBox>(tile->getComponent<CAnimation>().animation.getSize());
            tile->addComponent<CTransform>(gridToMidPixel(gridX, gridY, tile));
        }
        else if (temp == "Dec")
        {
            std::string animName;
            float gridX, gridY;
            fin >> animName >> gridX >> gridY;                      // Animation name, grid position (x,y) = (64 x 64px)
            
            // create tile entity WITHOUT bounding box
            auto tile = m_entityManager.addEntity("tile");
            
            tile->addComponent<CAnimation>(m_game->assets().getAnimation(animName), true);
            tile->addComponent<CTransform>(gridToMidPixel(gridX, gridY, tile));
        }
        else if (temp == "Player")
        {
            fin >> m_playerConfig.CHARACTER                         // Player character texture
                >> m_playerConfig.X >> m_playerConfig.Y             // Player starting position (x, y)
                >> m_playerConfig.CX >> m_playerConfig.CY           // Player collision box height, width (x, y)
                >> m_playerConfig.SPEED >> m_playerConfig.MAXSPEED  // Player horizontal move speed, max speed (counteract huge speeds due to gravity)
                >> m_playerConfig.JUMP >> m_playerConfig.MAXJUMP    // Player vertical jump speed (-y = up), gravity (+y = down)
                >> m_playerConfig.GRAVITY;
            
            spawnPlayer();
        }
        else if (temp == "Weapon")
        {
            fin >> m_weaponConfig.WEAPON                            // Weapon bullet texture
                >> m_weaponConfig.SPEED                             // Bullet speed (pixels/frame)
                >> m_weaponConfig.LIFESPAN;                         // Bullet lifespan (frames)
        }
    }
}

Vec2 Scene_Play::gridToMidPixel(float gridX, float gridY, std::shared_ptr<Entity> entity)
{
    // one grid is 64 x 64 pixels, entity center is at offset (x/2, y/2)
    float x = (gridX * 64) + entity->getComponent<CAnimation>().animation.getSize().x / 2.0f;
    float y = (gridY * 64) + entity->getComponent<CAnimation>().animation.getSize().y / 2.0f;

    // level grid y-axis and window y-axis are inverted
    y = m_game->window().getSize().y - y;

    return Vec2(x, y);
}

void Scene_Play::spawnPlayer()
{
    auto player = m_entityManager.addEntity("player");

    // Player properties set based on PlayerConfig struct
    player->addComponent<CAnimation>(m_game->assets().getAnimation(m_playerConfig.CHARACTER), true);
    player->addComponent<CBoundingBox>(Vec2(m_playerConfig.CX, m_playerConfig.CY));
    player->addComponent<CTransform>(   gridToMidPixel(m_playerConfig.X, m_playerConfig.Y, player),
                                        Vec2(m_playerConfig.SPEED, m_playerConfig.SPEED),
                                        0.0f);
    player->addComponent<CGravity>(m_playerConfig.GRAVITY);
    player->addComponent<CInput>();
    player->addComponent<CState>();

    m_player = player;
}

void Scene_Play::spawnBullet()
{
    auto bullet = m_entityManager.addEntity("bullet");

    // calculate player direction (+: right, -: left)
    float direction = (m_player->getComponent<CTransform>().scale.x > 0) ? 1 : -1;

    // Player properties set based on WeaponConfig struct
    auto anim = m_game->assets().getAnimation(m_weaponConfig.WEAPON);
    bullet->addComponent<CAnimation>(anim, true);
    bullet->addComponent<CBoundingBox>(Vec2(anim.getSize().x, anim.getSize().y));
    bullet->addComponent<CTransform>(   Vec2(m_player->getComponent<CTransform>().pos.x + m_player->getComponent<CBoundingBox>().halfSize.x * direction,
                                             m_player->getComponent<CTransform>().pos.y),
                                        Vec2(m_weaponConfig.SPEED * direction, 0),
                                        0.0f);
    bullet->addComponent<CLifespan>(m_weaponConfig.LIFESPAN, m_currentFrame);
}

void Scene_Play::update()
{
    m_entityManager.update();

    if (!m_paused)
    {
        sMovement();
        sCollision();
        sLifespan();
        sAnimation();    
    }
    sRender();
}

void Scene_Play::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
             if (action.name() == "TOGGLE_TEXTURE")     { m_drawTextures = !m_drawTextures; }
        else if (action.name() == "TOGGLE_COLLISION")   { m_drawCollision = !m_drawCollision; }
        else if (action.name() == "TOGGLE_GRID")        { m_drawGrid = !m_drawGrid; }
        else if (action.name() == "LEFT")               { m_player->getComponent<CInput>().left      = true; }
        else if (action.name() == "RIGHT")              { m_player->getComponent<CInput>().right     = true; }
        else if (action.name() == "JUMP")               {
                                                            if (m_player->getComponent<CInput>().canJump  == true) 
                                                            {
                                                                m_player->getComponent<CInput>().canJump   = false;
                                                                m_player->getComponent<CInput>().up        = true;
                                                            }
                                                        }
        else if (action.name() == "SHOOT")              {
                                                            if (m_player->getComponent<CInput>().canShoot)
                                                            {
                                                                m_player->getComponent<CInput>().shoot = true;
                                                                m_player->getComponent<CInput>().canShoot  = false;
                                                            }
                                                        }
        else if (action.name() == "QUIT")               { onEnd(); }
        else if (action.name() == "PAUSE")              { setPaused(!m_paused); }
    }
    else if (action.type() == "END")
    {
             if (action.name() == "LEFT")               { m_player->getComponent<CInput>().left      = false; }
        else if (action.name() == "RIGHT")              { m_player->getComponent<CInput>().right     = false; }
        else if (action.name() == "JUMP")               { m_player->getComponent<CInput>().up        = false; }
        else if (action.name() == "SHOOT")              { m_player->getComponent<CInput>().canShoot  = true; }
    }
}

void Scene_Play::sMovement()
{
    // Set player velocity based on input
    Vec2 playerVelocity = {0, m_player->getComponent<CTransform>().velocity.y};

    if (m_player->getComponent<CInput>().shoot)     { spawnBullet(); m_player->getComponent<CInput>().shoot =false; }
    if (m_player->getComponent<CInput>().left)      { playerVelocity.x = -m_playerConfig.SPEED; }
    if (m_player->getComponent<CInput>().right)     { playerVelocity.x =  m_playerConfig.SPEED; }
    if (m_player->getComponent<CInput>().up)        {
                                                        if (m_player->getComponent<CInput>().canJump)
                                                        {
                                                            float& jumpDur = m_player->getComponent<CState>().jumpDuration;
                                                            if (((jumpDur == 0) && !(m_player->getComponent<CState>().state == "air")) ||   // Check to start jump
                                                                ((jumpDur > 0) && (jumpDur < m_playerConfig.MAXJUMP) && (m_player->getComponent<CInput>().canJump)))                      // Check to continue jump
                                                            {
                                                                playerVelocity.y =  m_playerConfig.JUMP;
                                                                m_player->getComponent<CState>().jumpDuration += 1;
                                                                std::cout << "jump: " << jumpDur << "/" << m_playerConfig.MAXJUMP << std::endl;
                                                            }
                                                        }
                                                    }
                                                         
    m_player->getComponent<CTransform>().velocity = playerVelocity;

    // Update positions based on velocity
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CGravity>()) 
        {
            // Accelerate down (+y) for gravity, then cap max downward velocity so player doesn't pass through tile bounding boxes
            e->getComponent<CTransform>().velocity.y += e->getComponent<CGravity>().gravity;
            e->getComponent<CTransform>().velocity.y  = fminf(e->getComponent<CTransform>().velocity.y, m_playerConfig.MAXSPEED);
        }

        //Vec2& playerVelocity = m_player->getComponent<CTransform>().velocity;
        //playerVelocity.x = fmin(playerVelocity.x, m_playerConfig.MAXSPEED);
        //playerVelocity.y = fmin(playerVelocity.y, m_playerConfig.MAXSPEED);
        
        e->getComponent<CTransform>().pos += e->getComponent<CTransform>().velocity;
    }
}

void Scene_Play::sCollision()
{
    // Prevent player from going out of left side of window
    if (m_player->getComponent<CTransform>().pos.x - m_player->getComponent<CBoundingBox>().halfSize.x < 0)
    {
        m_player->getComponent<CTransform>().pos.x = m_player->getComponent<CBoundingBox>().halfSize.x;
    }
    
    // Reload level if player has fallen below the screen (dies)
    if (m_player->getComponent<CTransform>().pos.y + m_player->getComponent<CBoundingBox>().halfSize.y > m_game->window().getSize().y)
    {
        loadLevel(m_levelPath);
    }

    // TILES
    for (auto tile : m_entityManager.getEntities("tile"))
    {
        if (!(tile->hasComponent<CBoundingBox>())) {continue;}
        
        // PLAYER & TILES
            Vec2 overlap          = Physics::getOverlap(m_player, tile);
            Vec2 previousOverlap  = Physics::getPreviousOverlap(m_player, tile);

        // typedef for readability
            auto& playerTransform = m_player->getComponent<CTransform>();
            auto& tileTransform   = tile->getComponent<CTransform>();
            auto& tileType = tile->getComponent<CAnimation>().animation.getName();

        if (Physics::isCollision(overlap))
        {
            // collide from ABOVE
            if (previousOverlap.x > 0 && playerTransform.prevPos.y < tileTransform.prevPos.y)
            {
                playerTransform.pos.y -= overlap.y;                                                                  // adjust position by overlap
                playerTransform.velocity.y = 0;                                                                      // adjust velocity = 0
                m_player->getComponent<CInput>().canJump = true;                                                     // allow next jump
                m_player->getComponent<CState>().jumpDuration = 0;                                                   // |->  reset jumpDuration
                m_player->getComponent<CState>().state = (playerTransform.velocity.x != 0) ? "running" : "standing"; // set state for animation                
            }
            // collide from BELOW
            else if (previousOverlap.x > 0 && playerTransform.prevPos.y > tileTransform.prevPos.y)
            {
                playerTransform.pos.y += overlap.y;                                                                  // adjust position by overlap
                playerTransform.velocity.y = 0;                                                                      // adjust velocity = 0
                m_player->getComponent<CState>().jumpDuration = m_playerConfig.MAXJUMP;                              // stop current jump                   
                
                if (tileType == "Question")
                {
                    // Create a Coin tile one grid (64x64px) above the Question box position (tilePos), repeating = false
                    auto coin = m_entityManager.addEntity("tile");
                    auto tilePos = tile->getComponent<CTransform>().pos;

                    coin->addComponent<CAnimation>(m_game->assets().getAnimation("Coin"), false);
                    coin->addComponent<CTransform>(Vec2(tilePos.x, tilePos.y - tile->getComponent<CBoundingBox>().size.y));

                    // Change Question box animation from blinking to steady. Won't trigger again because tileType is different
                    tile->addComponent<CAnimation>(m_game->assets().getAnimation("Question2"), true);
                }
                else if (tileType == "Brick")
                {
                    // No animation for Brick destruction when hit by player from below
                    tile->destroy();
                }
            }
            
            // collide from the LEFT
            if (previousOverlap.y > 0 && playerTransform.prevPos.x < tileTransform.prevPos.x) { playerTransform.pos.x -= overlap.x; }

            // collide from the RIGHT
            else if (previousOverlap.y > 0 && playerTransform.prevPos.x > tileTransform.prevPos.x) { playerTransform.pos.x += overlap.x; }
        }    

        // BULLETS & TILES
        for (auto& bullet : m_entityManager.getEntities("bullet"))
        {
            Vec2 overlap          = Physics::getOverlap(bullet, tile);

            // typedef for readability
            auto& playerTransform = bullet->getComponent<CTransform>();
            auto& tileTransform   = tile->getComponent<CTransform>();
            auto& tileType = tile->getComponent<CAnimation>().animation.getName();

            if (Physics::isCollision(overlap))
            {
                bullet->destroy();
                
                if (tileType == "Brick")
                {
                    // Remove and replace Animation component. Explosion Animation set to repeating = false
                    tile->removeComponent<CAnimation>();
                    tile->addComponent<CAnimation>(m_game->assets().getAnimation("Explosion"), false);
                
                    // Remove BoundingBox component so player can move through tile even while Explosion Animation plays
                    tile->removeComponent<CBoundingBox>();
                }
            }
        }
    }
    
    // Movement and Collisions are done -> update prevPos
    m_player->getComponent<CTransform>().prevPos = m_player->getComponent<CTransform>().pos;
}

void Scene_Play::sLifespan()
{
    for (auto e : m_entityManager.getEntities())
    {
        if (e->hasComponent<CLifespan>() && !m_paused)
        {
            if (e->getComponent<CLifespan>().lifespan == 0) { e->destroy(); }
            else { e->getComponent<CLifespan>().lifespan--; }
        }
    }
}

void Scene_Play::sAnimation()
{
    for (auto e : m_entityManager.getEntities())
    {
        // Skip all entities without Animation component
        if (!e->hasComponent<CAnimation>()) {continue;}

        // Player animations
        if (e == m_player)
        {
            // Get current animation, state, and direction player is facing (scale)
            auto& playerAnimation = m_player->getComponent<CAnimation>().animation;
            std::string currentState = m_player->getComponent<CState>().state;
            auto currentScale = m_player->getComponent<CTransform>().scale;
            
            // Select animation to match state without reloading same state
            if (currentState == "standing" && playerAnimation.getName() != "Stand") 
            {
                playerAnimation = m_game->assets().getAnimation("Stand");
            }
            else if (currentState == "running" && playerAnimation.getName() != "Run")
            {
                playerAnimation = m_game->assets().getAnimation("Run");
            }
            else if (currentState == "air" && playerAnimation.getName() != "Air")
            {
                playerAnimation = m_game->assets().getAnimation("Air");
            }

            // Set player direction to previous player direction
            m_player->getComponent<CTransform>().scale = currentScale;

            // Set check if in the air (jumping or falling)
            if (m_player->getComponent<CTransform>().velocity.y != 0)
            {
                m_player->getComponent<CState>().state = "air";
            }

            // Set direction player is facing and set running or standing
            if ((m_player->getComponent<CInput>().left || m_player->getComponent<CInput>().right))
            {
                if (m_player->getComponent<CState>().state != "air")
                {
                    m_player->getComponent<CState>().state = "running";
                }

                int left = m_player->getComponent<CInput>().left ? -1 : 1;
                m_player->getComponent<CTransform>().scale.x = (fabsf(m_player->getComponent<CTransform>().scale.x) * left); 
            }
            else if (m_player->getComponent<CState>().state != "air")
            {
                m_player->getComponent<CState>().state = "standing";
            }
        }

        // Update animation for ALL entities
        auto& animation = e->getComponent<CAnimation>().animation;
        animation.update();

        // Animation clean-up
        if ((!e->getComponent<CAnimation>().repeating) && e->getComponent<CAnimation>().animation.hasEnded()) 
        {
            e->destroy();
        }
    }
}

void Scene_Play::sRender()
{
    // Clear the window to a blue
    m_game->window().setView(m_game->window().getDefaultView());
    m_game->window().clear(sf::Color(100, 100, 255));

    // Horizontal scrolling
    auto pPos = m_player->getComponent<CTransform>().pos;
    float windowCenterX = fmax(m_game->window().getSize().x / 2.0f, pPos.x);
    sf::View view = m_game->window().getView();
    view.setCenter(windowCenterX, view.getCenter().y); //m_game->window().getSize().y - view.getCenter().y
    m_game->window().setView(view);

    // Entity rendering and animation
    if (m_drawTextures)
    {
        for (auto e : m_entityManager.getEntities())
        {
            auto& transform = e->getComponent<CTransform>();

            if (e->hasComponent<CAnimation>())
            {
                auto& animation = e->getComponent<CAnimation>().animation;
                animation.getSprite().setRotation(transform.angle);
                animation.getSprite().setPosition(transform.pos.x, transform.pos.y);
                animation.getSprite().setScale(transform.scale.x, transform.scale.y);
                m_game->window().draw(animation.getSprite());
            }
            //m_game->window().draw(e->getComponent<CAnimation>().animation.getSprite());
        }
    }

    if (m_drawCollision)
    {
        for (auto e : m_entityManager.getEntities())
        {
            if (e->hasComponent<CBoundingBox>())
            {
                auto& box = e->getComponent<CBoundingBox>();
                auto& transform = e->getComponent<CTransform>();

                sf::RectangleShape rect;
                rect.setSize(sf::Vector2f(box.size.x-1, box.size.y-1));
                rect.setOrigin(sf::Vector2f(box.halfSize.x, box.halfSize.y));
                rect.setPosition(transform.pos.x, transform.pos.y);
                rect.setFillColor(sf::Color(0,0,0,0));
                rect.setOutlineColor(sf::Color(255,255,255,255));
                rect.setOutlineThickness(1);
                m_game->window().draw(rect);
            }
        }
    }

    /* 
    Don't have drawLine() function etc.
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - width() / 2;
        float rightX = leftX + width() + m_gridSize.x;
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.x);

        for (float x = nextGridX; x < rightX; x += m_gridSize.x)
        {
            drawLine(Vec2(x,0), Vec2(x, height()));
        }
        
        for (float y = 0; y < height(); y += m_gridSize.y)
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.x)
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.x);
                std::string xCell = std::to_string((int)y / (int)m_gridSize.y);
                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.y + 2);
                m_game->window().draw(m_gridText);
            }
        }
    }
    */

    m_game->window().display();
}

void Scene_Play::onEnd()
{
    m_game->changeScene("MENU", std::make_shared<Scene_Menu>(m_game));
}