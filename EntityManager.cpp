#include "EntityManager.h"

EntityManager::EntityManager() {}

void EntityManager::update()
{
    //Create new entities
    for (auto e : m_toAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->tag()].push_back(e);
    }
    m_toAdd.clear();

    // Remove dead entities from EntityVec and all EntityVec's inside EntityMap
    removeDeadEntities(m_entities);
    for (auto& [tag, entityVec] : m_entityMap)
    {
        removeDeadEntities(entityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    // Use std::remove_if to avoid iterator invalidation
    auto newItr = std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> entity) {return !entity->isActive();});
    vec.erase(newItr, vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    //Doesn't work because Entity constructor is private
    //auto e = std::make_shared<Entity>(tag, m_totalEntities++);
    auto e = std::shared_ptr<Entity>(new Entity(tag, m_totalEntities++));
    m_toAdd.push_back(e);
    return e;
}

EntityVec& EntityManager::getEntities() {return m_entities;}

EntityVec& EntityManager::getEntities(const std::string& tag) {return m_entityMap[tag];}

/*
int main()
{
    std::cout << "Creating EntityManager" << std::endl;
    EntityManager em = EntityManager();

    std::cout << "Creating e1 player" << std::endl;
    auto entity1 = em.addEntity("player");
    entity1->addComponent<CTransform>(Vec2(100, 150), Vec2(0,0), 0);

    std::cout << "Creating e2 player" << std::endl;
    auto entity2 = em.addEntity("player");
    entity2->addComponent<CTransform>(Vec2(100, 200), Vec2(0,0), 0);

    em.update();

    for (auto e : em.getEntities("player"))
    {

        std::cout << e->getComponent<CTransform>().pos.x << ", " << e->getComponent<CTransform>().pos.y << std::endl;
    }

    float dist = entity1->getComponent<CTransform>().pos.dist(entity2->getComponent<CTransform>().pos);
    std::cout << dist << std::endl;
    
    return 0;
}
*/