#pragma once

#include "Utils.hpp"

/*
    ������ �������� - ��� ��������������� ������, ������� ���� ������� ����� IEntity.
    ���� ��� �������� ��������.
    EntityComp - ���������� ����������� ������. ���������� �������� � �������
 
    ������ � ����������� ����� �� ���������, �� ����� ������������ �������� ����� �������� ������ � ������� ��� ������� ������� ��������
    ��� ����� ������ GetComponent<T>() � GetComponentPtr<T>()

    ��������! � �������� ������� ������ ���. ��� ���� ��������� ��� �����������. � ������ ����� ����������� ��� ������� �� ������.

    �����������, ����������� � ���������� �� ���������! ���� ���������� ���������� ������ - ����������� ������� ��� ��������

    ������:

    using SpriteEntity = EntityComp<SpriteComponent, TransformComponent>;
    SpriteEntity entity;
    entity.GetComponent<SpriteComponent>().SetTexture(new Texture("texture.png"));
*/

namespace StaticECS
{
    /*
        ������� ��������. ����� ������������� ��� ������������� ���������.
        ����� � ������� ���������� is_enable, ������� �� ��������� true.
        ������������ ����� �������������� ������������� � ��� � ��� ���� 
        ������������. �� ��������� ��� ���� �����, ������������� ��� ����
        ��� �� ���������� ������ ������� ���������.
    */
    struct IEntity 
    {
        bool is_enable = true; 
    };

    /*
        ����������� ��������.
        TComp... - ������ �����������, ������� ���������� �������� �� ������. 
        ����������� ���������� � ����� ������� �������.

        ������ � ����������� ���������� ���� ����� ����� std::get<T> ��� ���� _components
        ���� ����� ����� GetComponent<T>()
    */
    template<class ...TComp>
    struct EntityComp : public IEntity
    {
        using ComponentContainer = std::tuple<TComp...>;
        EntityComp() : IEntity()
        {
            ///������� ������ � ����������� ��������� �������� �����������
            for_each_in_tuple(_components, [this](auto &comp) 
            { 
                comp.SetParent(this); 
                comp.InitFromPool();
            });
        }
        ComponentContainer _components;

        ///�������� ������ �� ��������� �� ��������
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
        
        ///���� ����� ��� � �����, �� �������� ���������
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