#pragma once

#include <unordered_map>

/*
    Шаблонные компоненты.
    ComponentPool<Component> -  автоматически генерируемый пул компонентов. У каждого типа компонентов есть свой пул компонентов.
                                Пул не хранит сами компоненты, он лишь позволяет быстро найти компонент по индетификатору отца к-
                                омпонента.

                                Пул компонентов является синглтоном. Используйте Instance() для получения доступа к пулу.

    Component<Data>         -   Простейший шаблон компонента. Имеет ::Pool алиас, для доступа к пулу, связанного с этим типом. Об-
                                ычно компоненты сами добавляются в пул. Так же компоненты генерируются внутри Entity самостоятельно
                                без участия пользователя. Если же вы хотите создавать компоненты и использовать в своих собственных 
                                сущностях, то убедитесь, что parent не равен nullptr и что вы вызываете самостоятельно InitFromPool

    ВНИМАНИЕ! Компоненты не хранят в себе логики! Компонент - это данные. Вся логика считается в системах!
*/
namespace StaticECS
{
    class IEntity;

    /*
        Пул компонентов.
        Обычно компоненты самостоятельно добавляются в пул, без участия пользователя.
        Ключем компонента является указатель на отца, тем самомы, разные компоненты можно сопоставить по общему отцу
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

        ///Заполучить компонент через указания родителя
        ///Если не найдено - вернуть нулевой указатель
        TComponent* GetComponent(key_t key)
        {
            auto find = _pool.find(key);
            if (find != _pool.end())
            {
                return find->second;
            }

            return nullptr;
        }

        ///Добавляем компонент в пул
        ///Если родитель не указан - игнорируем
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

        ///Удалить компонент из пула.
        ///Если компонент не найден - ничего не происходит.
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
        Шаблонный компонент. Не имеет общего базового класса, тем самым, удебен для множественного наследования
        при агрегации базовых классов

        TData - простейшая структура с данными. Обычно имеет некоторое количество методов, для работы с данными.
                какой либо логики в компоненте быть не должно. Это просто данные для работы систем.
    */
    template<class TData>
    struct Component : public TData
    {
        ///Алиас на пул, связанный с данным типом компонентов. Можно использовать как
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

        ///ВНИМАНИЕ! Если вы создаете свой пул компонентов или логику0
        ///перегрузите данный метод!
        virtual void InitFromPool()
        {
            Pool::Instance().AddComponent(this);
        }

        ///ВНИМАНИ! Если вы создаете свой пул компонентов или логику
        ///перегрузите этот метод, что бы компонент не пытался себя 
        ///удалить там, где этгго не надо
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

        ///родительский индетификатор. Нужен что бы определить принадлежность того или иного компонента 
        ///к общему родителю

        friend Pool;
        IEntity *_parent = nullptr;
    };

}