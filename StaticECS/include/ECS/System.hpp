#pragma once

/*
    Простейшие базовые системы.
    Так как системы и компоненты очень слабо-связны, систему можно определить самостоятельно
    без использования данных шаблонов.

    ВНИМАНИЕ! По концепту СИТСТЕМ и только Систма должна обрабатывать логику компонентов и отвечать за меж-компонентное
    взаимодействие. Система может быть как полноценным обьектом. Так и простой функцией (см. пример)

    пример1:

    class MySystem : System<MyComponent>
    {
    public:
        ...

        void MyLogic();
    }

    ...
    MyStstem system;
    system.MyLogic();

    пример2:
    //Система может быть просто функцией, выполняющейся где нибудь в своем потоке, перебирающая свои собственные компоненты
    void MySystem()
    {
        auto& components = MyComponent::Pool::Instance().GetPool();

        for (auto &&v : components)
        {
            auto dmg = v.second->damage; 
            ...
        }
    }
*/
namespace StaticECS
{
    /*
        Простенькая абстрактная система с методами Draw и Update.
        Так же имее алиас на пул компонентов, которыми управляет
    */
    template<class TComponent>
    struct System 
    {
        using Pool = typename TComponent::Pool;
        
        virtual void Update(float) = 0;
        virtual void Draw() = 0;

    };
}