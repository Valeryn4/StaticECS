#pragma once


//Static Entity Component System
/*
    облегченная реализация ECS, через агегирующие шаблоны и явную композицию через кортежи. Тут все отдано на откуп программисту. 
    Создание Entity, наполнение его компонентами и получение компонентов - все просчитывается в Компайл тайме.
    Данная библиотека не содержит в себе готовых компонентов или систем. Только базовый функционал!!!

    EntityComp<Components...> -     Сущность-композитор. Все данные хранятся в кортеже, внутри сущности. 
                                    Доступ к ним вызывает std::get<T>.

    Component<DATA> -   Шаблонный компонент. Каждый тип компонента уникальный и не имеет базового общего 
                        класса. DATA - это любая структура с данными и методами для работы с данными. В 
                        компоненьте ни в коме случае, не должна выполняться логика!!!! Только базовый 
                        фунционал для работы с данными.

    System<Component> - Шаблонная система связанная с компонентом. Это абстрактная система с методами Update
                        и Draw. Как система будет работать и взаимоействовать с компонентами - остается на 
                        откуп программисту. Т.к. сущности и компоненты не знают ничего об системах, систему
                        можно реализовать независимо от фреймворка, не наследуя ни от какого либо класса

    Component<DATA>::Pool - Пулл компонентов. Имеет статический инстанс. В основном, это необходимо системе.

    EventComponent
    SystemEvent     - Простейшая система событий, где событие - есть функция передающайся в std::function<void()>
                        

    пример:

    Render::Texture _texture;

    using Sprite = Component<SpriteData>;
    using Transform = Component<TransformData>;
    using MoveTo = Component<MoveToData>;

    using Entity = EntityComp<Sprite, Transform, MoveTo>;

    SpriteSystem _spriteSystem;
    MoveSystem _moveSystem;

    Entity _entity;

    void Init()
    {
        _entity.GetComponent<Sprite>().SetTexture(_texture);
        _entity.GetComponent<Transform>().SetPosition(math::Vector3(500,500,500));
        _entity.GetComponent<MoveTo>().SetParam(math::Vector3(900, 900, 900), 10.f);
    }

    void Draw()
    {
        //отрисовка компонентов спрайт
        _spriteSystem.Draw();
    }

    void Update(float dt)
    {
        //обновлени движения сущностей.
        _moveSystem.Update(dt); 
    }
*/

#include "System.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Event.hpp"
#include "Utils.hpp"