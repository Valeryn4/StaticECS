#pragma once

#include <unordered_map>

/*
    ��������� ����������.
    ComponentPool<Component> -  ������������� ������������ ��� �����������. � ������� ���� ����������� ���� ���� ��� �����������.
                                ��� �� ������ ���� ����������, �� ���� ��������� ������ ����� ��������� �� �������������� ���� �-
                                ���������.

                                ��� ����������� �������� ����������. ����������� Instance() ��� ��������� ������� � ����.

    Component<Data>         -   ���������� ������ ����������. ����� ::Pool �����, ��� ������� � ����, ���������� � ���� �����. ��-
                                ���� ���������� ���� ����������� � ���. ��� �� ���������� ������������ ������ Entity ��������������
                                ��� ������� ������������. ���� �� �� ������ ��������� ���������� � ������������ � ����� ����������� 
                                ���������, �� ���������, ��� parent �� ����� nullptr � ��� �� ��������� �������������� InitFromPool

    ��������! ���������� �� ������ � ���� ������! ��������� - ��� ������. ��� ������ ��������� � ��������!
*/
namespace StaticECS
{
    class IEntity;

    /*
        ��� �����������.
        ������ ���������� �������������� ����������� � ���, ��� ������� ������������.
        ������ ���������� �������� ��������� �� ����, ��� ������, ������ ���������� ����� ����������� �� ������ ����
    */
    template<class TComponent>
    class ComponentPool
    {
    public:
        using key_t = IEntity*;
        using ComponentMap = std::unordered_map<key_t, TComponent*>;

        static ComponentPool& Instance()
        {
            static ComponentPool inst;
            return inst;
        }

        ///���������� ��������� ����� �������� ��������
        ///���� �� ������� - ������� ������� ���������
        TComponent* GetComponent(key_t key)
        {
            auto find = _pool.find(key);
            if (find != _pool.end())
            {
                return find->second;
            }

            return nullptr;
        }

        ///��������� ��������� � ���
        ///���� �������� �� ������ - ����������
        void AddComponent(TComponent* comp)
        {
            if (comp->_parent)
            {
                auto res = _pool.emplace(comp->_parent, comp);
                if (res.second)
                {
                    
                }
            }
        }

        ///������� ��������� �� ����.
        ///���� ��������� �� ������ - ������ �� ����������.
        void RemoveComponent(TComponent* comp)
        {
            if (comp->_parent)
            {
                auto find = _pool.find(comp->_parent);
                if (find != _pool.end())
                {
                    _pool.erase(find);
                }
            }
        }

        ComponentMap& GetPool()
        {
            return _pool;
        }


    protected:
        ComponentMap _pool;
        ComponentPool() : _pool({}) {}
        ComponentPool(ComponentPool &&) = delete;
        ComponentPool(const ComponentPool &) = delete;
        ComponentPool& operator=(const ComponentPool &) = delete;

    };

    /*
        ��������� ���������. �� ����� ������ �������� ������, ��� �����, ������ ��� �������������� ������������
        ��� ��������� ������� �������

        TData - ���������� ��������� � �������. ������ ����� ��������� ���������� �������, ��� ������ � �������.
                ����� ���� ������ � ���������� ���� �� ������. ��� ������ ������ ��� ������ ������.
    */
    template<class TData>
    struct Component : public TData
    {
        ///����� �� ���, ��������� � ������ ����� �����������. ����� ������������ ���
        /// ::Pool::Instance().GetComponent(parent);
        Component() {}
        Component(IEntity *parent) : _parent(parent)
        {
            InitFromPool();
        }

        using Pool = ComponentPool<Component<TData>>;
        virtual ~Component()
        {
            RemoveFromPool();
        }
        
        IEntity *GetParent()
        {
            return _parent;
        }

        void SetParent(IEntity *parent)
        {
            _parent = parent;
        }

        ///��������! ���� �� �������� ���� ��� ����������� ��� ������0
        ///����������� ������ �����!
        virtual void InitFromPool()
        {
            Pool::Instance().AddComponent(this);
        }

        ///�������! ���� �� �������� ���� ��� ����������� ��� ������
        ///����������� ���� �����, ��� �� ��������� �� ������� ���� 
        ///������� ���, ��� ����� �� ����
        virtual void RemoveFromPool()
        {
            Pool::Instance().RemoveComponent(this);
        }

        static Pool& GetPool()
        {
            return Pool::Instance();
        }


    protected:
        Component(Component&&) = delete;
        Component(const Component &) = delete;
        Component& operator=(const Component &) = delete;

        ///������������ �������������. ����� ��� �� ���������� �������������� ���� ��� ����� ���������� 
        ///� ������ ��������

        friend Pool;
        IEntity *_parent = nullptr;
    };

}