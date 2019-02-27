#include "include/ECS/ECS.hpp"
#include <string>
#include <iostream>
#include <chrono>
#include <thread>




//������ ��� ����������-���������
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

//������ ��� ���������� ��������� ����������
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

//��������� ��������� ��������
using ListenerTargetComp = StaticECS::Component<ListenerTargetData>;
//��������� ��������� ����������
using ListenerBrodcastComp = StaticECS::Component< ListenerBrodcastData>;
//��������� event
using EventComponent = StaticECS::EventComponent;

//�������� ������������ ������� � ��������� ��������
using Entity = StaticECS::EntityComp< EventComponent, ListenerTargetComp>;
//�������� ��������� ���������� � ��������� �������� (��������! �� ���������� �������� ���� ���������-��������! ��� ������������)
using Entity2 = StaticECS::EntityComp<ListenerBrodcastComp, ListenerTargetComp>;




int main()
{
    //������� ��������, ������� ����� ������������ �������
    Entity entity;
    //������� �������
    StaticECS::SystemEvent system_;

    //�������� ��������� �� ���������� �����������
    auto listener = entity.GetComponentPtr<ListenerTargetComp>();
    auto event = entity.GetComponentPtr<EventComponent>(); //��������� event, � ������� �� ����� �������� �������


    //������� 10 ���������, ������� ����� ������ ���������, ���������� � ListenerBrodcastComp
    for (int i = 0; i < 10; ++i)
    {
        auto e = new Entity2();
        e->GetComponent<ListenerBrodcastComp>().name = "Brodcast_" + std::to_string(i);
    }

    
    //����� ��� ����� � ��� GameLoop
    std::thread thread([listener, event, &system_]() 
    {


        for (int i = 0; i < 10; ++i)
        {

            auto parent = event->GetParent();

            //���������� ��� ������� � ������� ��������� ������ ����. ���� �������� parent!
            for (int z = 0; z < 3; ++z)
            {
                
                event->PushEvent([parent]()
                {
                    //��� �� �������� ���������-���������, ������� ����������� ��� �� ��������, ��� � ������� �����
                    //���� ������ ���������� ��� - ������� nullptr
                    auto comp = ListenerTargetComp::Pool::Instance().GetComponent(parent); 
                    if (comp)
                    {
                        comp->Set(comp->i + 1);
                    }
                });

                
            }

            //���������� �������-��������, ������� ����������� ���� ����������� ListenerBrodcastComp
            event->PushEvent([]() 
            {
                auto& pool = ListenerBrodcastComp::Pool::Instance().GetPool();

                //�������� ��������! ��� ������� �������� ������� �� ���������� std::unorder_map
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