#pragma once

#include "Component.hpp"
#include "System.hpp"

#include <queue>
#include <functional>

/*
    ���������� ������� �������.
    ���� ������� � ���, ��� �� ������� - ���� ��������. ������� ������ �������, ����������� � �������. �� �������� �
    ��������� ��������� �� ������ ����� std::function<void()>. � ���������� ������� �������� ��� ������� ��� ������ Update()


    EventData - ��� ������ EventComponent. ����� ���������� ������ PushEvent � PopEvent.
                event_t - ��� �������, ������� � �������� ��������.

    EventComponent - ��� ������� ����� Component<EventData>. ������� ���� ��� �����������

    EventSystem - ���������� ������� �������. ��� ������ ������ Update - ���������� ��� ������� ���� ����������� EventComponent.


    ��� ����� �������, ��������� ������ �������. ����� �� ������ �������� ���� ������� �������.

    ������:

    ///ListenerComponent - ����� ����� ��������� ����������� � ����� "int i = 0" � "std::string name = "hello""
    using EntityListener = EntityComp<ListenerComponent>;

    using EntityEvent = EntityComp<EventComponent>;

    EntityListener listener;
    EntityEvent dispatcher;
    SystemEvent system;

    auto& event = dispatcher.GetComponent<EventComponent>();
    ///��� �����, �� ���������� ��� ���������� ListenerComponent � �������� ����� �� ��������.
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


    ������2:

    ///��� ���� �����, �� �� ����� ���������� ������� ���� ���� � ����� � ��� ����� ���� ��������

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

   
    �����!

    ���� �� ������ ������� ���� ������� � ������� ������� �� �������� ���� ��� ������ ����������� EventData � ������� ���� �������.
    � ����� �� ��������� ��� ���������� ���� �������� ������.
  
    
    ��� ������, ���� ������ �������. ������ � ���� �������. ��� ����� � ���� ������. ����� �� ������ ��� ������ ���� event_t.
    � �� ���� ������ �������� ����:
    template<typename TEvent, typename TQueue>
    struct EventData;

    template<class TEventData, typename UpdateFn>
    struct SystemEvent;

    ��� ��� ������� ����� �������, ������ � ������������� �� ��� ������. 
    � ������� ������� ����������� � ����� ������� � ���� �����.

    ������:
    void MyEventSystem()
    {
        auto& pool = MyEvent::Pool::Instance().GetPool();

        for (auto &&v : pool)
        {
            ...///������
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