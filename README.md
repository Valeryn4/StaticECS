# StaticECS
This Template ECS (Entity Component System)

облегченная реализация ECS, через агегирующие шаблоны и явную композицию через кортежи. Тут все отдано на откуп программисту. 
Создание Entity, наполнение его компонентами и получение компонентов - все просчитывается в Компайл тайме.
Данная библиотека не содержит в себе готовых компонентов или систем. Только базовый функционал!!!


  ```
  EntityComp<Components...>
  ```
  - Сущность-композитор. Все данные хранятся в кортеже, внутри сущности. Доступ к ним вызывает std::get<T>.
  ```
  Component<DATA></b> 
  ```
  -   Шаблонный компонент. Каждый тип компонента уникальный и не имеет базового общего класса. DATA - это любая структура с данными и методами для работы с данными. В компоненьте ни в коме случае, не должна выполняться логика!!!! Только базовый фунционал для работы с данными.

  ```
  System<Component>
  ```
  - Шаблонная система связанная с компонентом. Это абстрактная система с методами Update и Draw. Как система будет работать и взаимоействовать с компонентами - остается на откуп программисту. Т.к. сущности и компоненты не знают ничего об системах, систему можно реализовать независимо от фреймворка, не наследуя ни от какого либо класса

  ```
  Component<DATA>::Pool
  ```
  - Пулл компонентов. Имеет статический инстанс. В основном, это необходимо системе.
  ```
  EventComponent
  SystemEvent
  ```
  - Простейшая система событий, где событие - есть функция передающайся в std::function<void()>


пример:
```cpp
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
```

  пример с событиями:
```cpp




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
```
