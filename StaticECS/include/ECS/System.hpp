#pragma once

/*
    ���������� ������� �������.
    ��� ��� ������� � ���������� ����� �����-������, ������� ����� ���������� ��������������
    ��� ������������� ������ ��������.

    ��������! �� �������� ������� � ������ ������ ������ ������������ ������ ����������� � �������� �� ���-������������
    ��������������. ������� ����� ���� ��� ����������� ��������. ��� � ������� �������� (��. ������)

    ������1:

    class MySystem : System<MyComponent>
    {
    public:
        ...

        void MyLogic();
    }

    ...
    MyStstem system;
    system.MyLogic();

    ������2:
    //������� ����� ���� ������ ��������, ������������� ��� ������ � ����� ������, ������������ ���� ����������� ����������
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
        ����������� ����������� ������� � �������� Draw � Update.
        ��� �� ���� ����� �� ��� �����������, �������� ���������
    */
    template<class TComponent>
    struct System 
    {
        using Pool = typename TComponent::Pool;
        
        virtual void Update(float) = 0;
        virtual void Draw() = 0;

    };
}