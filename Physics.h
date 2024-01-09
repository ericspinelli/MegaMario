#pragma once

#include <memory>
#include <cmath>
#include "Entity.h"
#include "Components.h"
#include "Vec2.h"

namespace Physics
{
    Vec2 getOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    Vec2 getPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);

    bool isCollision(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b);
    bool isCollision(Vec2 overlap);
};