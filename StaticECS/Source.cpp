#include "include/ECS/ECS.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>




//Данные для компонента-слушателя
struct ListenerTargetData
{
    std::string name =  "Target_0";
    int i = 0;

    void Set(int v)
    {
        i = v;
        printf("Listener %s i set to %i\n", name.c_str(), i);
    }
};

//данные для компонента слушателя бродкастов
struct ListenerBrodcastData
{
    std::string name = "Brodcast";
    int i = 0;

    void Set(int v)
    {
        i = v;
        printf("ListenerBrodcast %s i set to %i\n", name.c_str(), i);
    }
};

//Компонент слушателя одиночки
using ListenerTargetComp = StaticECS::Component<ListenerTargetData>;
//компонент слушателя бродкастов
using ListenerBrodcastComp = StaticECS::Component< ListenerBrodcastData>;
//компонент event
using EventComponent = StaticECS::EventComponent;

//сущность генерирующая события и слушатель одиночка
using Entity = StaticECS::EntityComp< EventComponent, ListenerTargetComp>;
//сущность слушатель бродкастов и слушатель одиночка (ВНИМАНИЕ! МЫ СПЕЦИАЛЬНО ДОБАВИЛИ СЮДА СЛУШАТЕЛЯ-ОДИНОЧКУ! ДЛЯ ДЕМОНСТРАЦИИ)
using Entity2 = StaticECS::EntityComp<ListenerBrodcastComp, ListenerTargetComp>;




int main()
{
    //создаем сущность, которая будет генерировать события
    Entity entity;
    //создаем систему
    StaticECS::SystemEvent system_;

    //получаем указатели на компоненты бродкастера
    auto listener = entity.GetComponentPtr<ListenerTargetComp>();
    auto event = entity.GetComponentPtr<EventComponent>(); //компонент event, в который мы будем посылать кобытия


    //создаем 10 сущностей, которые будут ловить бродкасты, посылаемые в ListenerBrodcastComp
    for (int i = 0; i < 10; ++i)
    {
        auto e = new Entity2();
        e->GetComponent<ListenerBrodcastComp>().name = "Brodcast_" + std::to_string(i);
    }

    
    //Пусть это будет у наж GameLoop
    std::thread thread([listener, event, &system_]() 
    {


        for (int i = 0; i < 10; ++i)
        {

            auto parent = event->GetParent();

            //отправляем три события в целевой компонент самому себе. Ключ является parent!
            for (int z = 0; z < 3; ++z)
            {
                
                event->PushEvent([parent]()
                {
                    //так мы получаем компонент-слушатель, который принадлежит той же сущности, что и создает ивент
                    //если такого компонента нет - получим nullptr
                    auto comp = ListenerTargetComp::Pool::Instance().GetComponent(parent); 
                    if (comp)
                    {
                        comp->Set(comp->i + 1);
                    }
                });

                
            }

            //отправляем событие-бродкаст, которео отправиться всем компонентам ListenerBrodcastComp
            event->PushEvent([]() 
            {
                auto& pool = ListenerBrodcastComp::Pool::Instance().GetPool();

                //обратиет внимание! Что порядок перебора зависит от реализации std::unorder_map
                for (auto &&v : pool)
                {
                    v.second->Set(v.second->i + 1);
                }
                
            });

            printf("\nUpdate all events!\n");
            system_.Update(1);

            using namespace std::chrono_literals;
            std::this_thread::sleep_for(1s);
        }

    });


    thread.join();

    system("pause");
}