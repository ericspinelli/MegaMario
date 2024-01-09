#pragma once

#include <tuple>
#include <memory>
#include <string>

#include "Components.h"

class EntityManager;

typedef std::tuple<
    CTransform,
    CLifespan,
    CInput,
    CBoundingBox,
    CAnimation,
    CGravity,
    CState
> ComponentTuple;

class Entity
{
    friend class EntityManager;
    
    const size_t        m_id    = 0;
    const std::string   m_tag   = "Default";
    bool                m_alive = true;
    ComponentTuple      m_components;

    Entity(const std::string& tag, const size_t id)
        : m_tag(tag)
        , m_id(id)
    {}

public:
    void                destroy()           {m_alive = false;}
    size_t              id()        const   {return m_id;}
    bool                isActive()  const   {return m_alive;}
    const std::string&  tag()       const   {return m_tag;}
    
    template <typename T>
    bool hasComponent()
    {
        return getComponent<T>().has;
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template<typename T>
    T& getComponent()
    {
        return std::get<T>(m_components);
    }

    template<typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};