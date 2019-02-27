#pragma once

#include "Utils.hpp"

/*
    Каждая сущность - это самодостаточный обьект, имеющий лишь базовый класс IEntity.

    EntityComp - использует композитный подход. Компоненты хранятся в кортеже
 
    Доступ к компонентам никак не ограничен, по этому пользователь свободно может получить доступ к кортежу или базовым классам напрямую
    или через методы GetComponent<T>() и GetComponentPtr<T>()

    ВНИМАНИЕ! У сущности никакой логики нет. Это лишь контейнер для компонентов. И ничего кроме компонентов оно хранить не должно.

    Копирование, перемещение и присвоение не запрещены! Если необходимо передавать обьект - используйте обертки или поинтеры

    пример:

    using SpriteEntity = EntityComp<SpriteComponent, TransformComponent>;
    SpriteEntity entity;
    entity.GetComponent<SpriteComponent>().SetTexture(new Texture("texture.png"));
*/

namespace StaticECS
{
    /*
        Базовая сущность. Нужна исключительно для индетификации сущностей.
        имеет в наличии переменную is_enable, которая по умолчанию true.
        Пользователь может самостоятельно распорядиться с ней и как либо  
        использовать. По умолчанию она лишь нужна, исключительно для того
        что бы определить размер базовой структуры.
    */
    struct IEntity 
    {
        bool is_enable = true; 
    };

    /*
        Композитная сущность.
        TComp... - список компонентов, которые необходимо передать во внутрь. 
        Допускаются компоненты с общим базовым классом.

        Доступ к компонентов происходит либо через вызов std::get<T> для поля _components
        либо через метод GetComponent<T>()
    */
    template<class ...TComp>
    struct EntityComp : public IEntity
    {
        using ComponentContainer = std::tuple<TComp...>;
        EntityComp() : IEntity()
        {
            ///Обходим кортеж и присваиваем стартовые значения компонентам
            for_each_in_tuple(_components, [this](auto &comp) 
            { 
                comp.SetParent(this); 
                comp.InitFromPool();
            });
        }
        ComponentContainer _components;

        ///получаем ссылку на компонент из колекции
        template<class T>
        T& GetComponent()
        {
            return std::get<T>(_components);
        }

        template<class T>
        bool HasComponent()
        {
            return tuple_contains_type<T, decltype(_components)>::value;
        }
        
        ///тоже самое что и ранее, но получаем указатель
        template<class T>
        T* GetComponentPtr()
        {
            auto& v = std::get<T>(_components);
            return &v;
        }

    protected:
        EntityComp(EntityComp &&) = delete;
        EntityComp(const EntityComp &) = delete;
        EntityComp& operator=(const EntityComp &) = delete;

    };

    
}
