#pragma once


//Static Entity Component System
/*
    ����������� ���������� ECS, ����� ����������� ������� � ����� ���������� ����� �������. ��� ��� ������ �� ����� ������������. 
    �������� Entity, ���������� ��� ������������ � ��������� ����������� - ��� �������������� � ������� �����.
    ������ ���������� �� �������� � ���� ������� ����������� ��� ������. ������ ������� ����������!!!

    EntityComp<Components...> -     ��������-����������. ��� ������ �������� � �������, ������ ��������. 
                                    ������ � ��� �������� std::get<T>.

    Component<DATA> -   ��������� ���������. ������ ��� ���������� ���������� � �� ����� �������� ������ 
                        ������. DATA - ��� ����� ��������� � ������� � �������� ��� ������ � �������. � 
                        ����������� �� � ���� ������, �� ������ ����������� ������!!!! ������ ������� 
                        ��������� ��� ������ � �������.

    System<Component> - ��������� ������� ��������� � �����������. ��� ����������� ������� � �������� Update
                        � Draw. ��� ������� ����� �������� � ���������������� � ������������ - �������� �� 
                        ����� ������������. �.�. �������� � ���������� �� ����� ������ �� ��������, �������
                        ����� ����������� ���������� �� ����������, �� �������� �� �� ������ ���� ������

    Component<DATA>::Pool - ���� �����������. ����� ����������� �������. � ��������, ��� ���������� �������.

    EventComponent
    SystemEvent     - ���������� ������� �������, ��� ������� - ���� ������� ������������ � std::function<void()>
                        

    ������:

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
        //��������� ����������� ������
        _spriteSystem.Draw();
    }

    void Update(float dt)
    {
        //��������� �������� ���������.
        _moveSystem.Update(dt);
    }
*/

#include "System.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include "Event.hpp"
#include "Utils.hpp"