#pragma once

#include "Component.hpp"
#include "System.hpp"

#include <queue>
#include <functional>

/*
    Простейшая система ивентов.
    Идея состоит в том, что бы событие - было функцией. Никакой ложной системы, подписчиков и прочего. Мы передаем в
    компонент указатель на фукцию через std::function<void()>. А обработчик событий выполнит эту функцию при вызове Update()


    EventData - это данные EventComponent. Имеет простейшие классы PushEvent и PopEvent.
                event_t - это функция, которая и является событием.

    EventComponent - это готовый шабло Component<EventData>. Имеющий свой пул компонентов

    EventSystem - простейшая система событий. При каждом вызове Update - опусташает все очереди всех компонентов EventComponent.


    Как можно увидеть, концепция крайне простая. Ничто не мешает написать свою систему событий.

    пример:

    ///ListenerComponent - пусть будет безымяным компонентом с полем "int i = 0" и "std::string name = "hello""
    using EntityListener = EntityComp<ListenerComponent>;

    using EntityEvent = EntityComp<EventComponent>;

    EntityListener listener;
    EntityEvent dispatcher;
    SystemEvent system;

    auto& event = dispatcher.GetComponent<EventComponent>();
    ///тем самым, мы опрашиваем все компоненты ListenerComponent и передаем какие то значения.
    event.PushEvent([]()
    {
        for (auto &&v : ListenerComponent::Pool::Instance().GetPool())
        {
            v.second->i++;
            printf("component %s changed i to %i\n", v.second->name, v.second->i);
        }
    });

    ...
    system.Update();

    ...
     >>> "component hello changed i to 1"


    пример2:

    ///все тоже самое, но мы хотим передавать событие сами себе и пусть у нас будет одна сущность

    using Entity = EntityComp<EventComponent, ListenerComponent>;
    Entity entity;
    ...
    auto& event = entity.GetComponent<EventComponent>();
    event.PushEvent([&event]()
    {
        auto& comp = ListenerComponent::Pool::Instance().GetComponent(event.GetParent());
        comp.i++;
        ...
    });

   
    ВАЖНО!

    Ничо не мешает создать свои системы и системы ивентов по аналогии ниже или просто перегрузить EventData и создать свою систему.
    И никто не запрещает вам передавать одни событиия другим.
  
    
    Как видите, идея крайне простая. Фунция и есть событие. Она несет в себе логику. Ничто не мешает вам задать свой event_t.
    Я не стал делать шаблонны типо:
    template<typename TEvent, typename TQueue>
    struct EventData;

    template<class TEventData, typename UpdateFn>
    struct SystemEvent;

    Так как система очень простая, легкая и переусложнять ее нет смысла. 
    А система событий реализуется в одной функции в пару строк.

    пример:
    void MyEventSystem()
    {
        auto& pool = MyEvent::Pool::Instance().GetPool();

        for (auto &&v : pool)
        {
            ...///логика
        }
    }
    
    void GameLoop()
    {
        MyEventSystem();
    }

*/
namespace StaticECS
{
    struct EventData
    {
        using event_t = std::function<void()>;
        using queue_t = std::queue<event_t>;

        queue_t events;

        inline void PushEvent(const event_t &ev)
        {
            events.push(ev);
        }

        inline event_t PopEvent()
        {
            auto ev = events.front();
            events.pop();
            return ev;
        }

        inline size_t Size()
        {
            return events.size();
        }

        inline bool Empty()
        {
            return events.empty();
        }
    };

    using EventComponent = Component< EventData >;

    struct SystemEvent
    {
        using Pool = EventComponent::Pool;
        inline void Update(float dt)
        {
            auto& pool = Pool::Instance().GetPool();

            for (auto &&v : pool)
            {
                auto ev = v.second;
                while (!ev->Empty())
                {
                    auto e = ev->PopEvent();
                    e();
                }
            }
        }

    };

}