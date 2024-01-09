#include "Physics.h"

Vec2 Physics::getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 aPos = a->getComponent<CTransform>().pos;
    Vec2 bPos = b->getComponent<CTransform>().pos;

    float dx = fabsf(aPos.x - bPos.x);
    float dy = fabsf(aPos.y - bPos.y); 
    
    Vec2 aHalfSize = a->getComponent<CBoundingBox>().halfSize;
    Vec2 bHalfSize = b->getComponent<CBoundingBox>().halfSize;

    return Vec2((aHalfSize.x + bHalfSize.x) - dx, (aHalfSize.y + bHalfSize.y) - dy);
}

Vec2 Physics::getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 aPos = a->getComponent<CTransform>().prevPos;
    Vec2 bPos = b->getComponent<CTransform>().prevPos;

    float dx = fabsf(aPos.x - bPos.x);
    float dy = fabsf(aPos.y - bPos.y); 
    
    Vec2 aHalfSize = a->getComponent<CBoundingBox>().halfSize;
    Vec2 bHalfSize = b->getComponent<CBoundingBox>().halfSize;

    return Vec2((aHalfSize.x + bHalfSize.x) - dx, (aHalfSize.y + bHalfSize.y) - dy);
}

bool Physics::isCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    Vec2 overlap = getOverlap(a, b);
    
    return ((overlap.x > 0) && (overlap.y > 0));
}

bool Physics::isCollision(Vec2 overlap)
{
    return ((overlap.x > 0) && (overlap.y > 0));
}