#include <mpi.h>
#include <list>
#include <iostream>
#include <thread>
#include <chrono>
#include "../../../modules/test_tasks/test_mpi/ops_mpi.h"


bool is_can_eat(const bool* forks,const int philosoph,int ph_num)
{
    return !(forks[philosoph-1] || forks[philosoph%ph_num]);
}

bool is_cache_free(const bool* cache,int size)
{
    for(int i =0;i<size;i++)
    {
        if(cache[i])
        {
            return false;
        }
    }
    return true;
}

void send_permission(bool* forks,int philosoph,int philosoph_num,bool mark)
{
    forks[philosoph-1] = mark;
    forks[philosoph%philosoph_num] = mark;
    const int permission = 1;
    if(mark)
    MPI_Send(&permission, 1, MPI_INT, philosoph, 0, MPI_COMM_WORLD);
}

int critical_code(int n, int process)
{
    MPI_Status status;
    int param;
    
    MPI_Recv(&param,1, MPI_INT, process, 0, MPI_COMM_WORLD, &status);
    // std::cout<<"ПРИНЯТЫЙ ПОТОК "<<process<<std::endl;
    // std::cout<<"ПРИНЯТЫЙ ПАРАМЕТР "<<param<<std::endl;
    
    return n+=param;
}



void philosoph(int rank,int param)
{
    MPI_Barrier(MPI_COMM_WORLD);
    // std::cout<<"Прерывание блокировки "<<rank<<std::endl;
    int permission = 0;
    int readiness = 1;
    MPI_Status status;

    MPI_Send(&readiness, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // std::cout<<"Отправка желания "<<rank<<std::endl;

    MPI_Recv(&permission,1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
    // std::cout<<"Прерывание блокировки "<<rank<<std::endl;

    if(permission==1)
    {
        MPI_Send(&param, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        // std::cout<<"ОТПРАВЛЯЕМЫЙ ПАРАМЕТР "<<param<<std::endl;

        int send_end = -1;
        MPI_Send(&send_end, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }
    // std::cout<<rank<<" __________________Завершил выполнение"<<std::endl;
    
}

int manager(int size)
{
    int philosoph_num = size-1;

    bool* forks = new bool[philosoph_num];

    for(int j = 0;j<philosoph_num;j++)
    forks[j]= false;
    std::list<int> turn;
    int i =0;
    int n = 0;
    MPI_Status status;
    int flag = 0;
    MPI_Barrier(MPI_COMM_WORLD);
    // std::cout<<"Потоки разблочены"<<std::endl;
    do
    {
        MPI_Recv(&flag,1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        const int th_source = status.MPI_SOURCE;

        //  std::cout<<"Поток "<<th_source<<" запросил разрешение........"<<std::endl;
        //  std::cout<<"Флаг = "<<flag<<" ........"<<std::endl;
        // std::cout<<"Значения вилок"<<std::endl;
        // std::cout<<"Forks = [";
        // for(int i =0;i<philosoph_num;i++)
        // std::cout<<forks[i]<<" ";
        // std::cout<<"]"<<std::endl;

        if(flag == 1)
        {
            i++;
            if(is_can_eat(forks,th_source,philosoph_num))
            {
                // std::cout<<"Потоку "<<th_source<<" разрешение дано........"<<std::endl;
                send_permission(forks,th_source,philosoph_num,true);
                n = critical_code(n,th_source);
                // std::cout<<"Критическая секция поток"<<th_source<<std::endl;
                // std::cout<<"N = "<<n<<std::endl;
                // std::cout<<"Значения вилок"<<std::endl;
                // std::cout<<"Forks = [";
                // for(int i =0;i<philosoph_num;i++)
                // std::cout<<forks[i]<<" ";
                // std::cout<<"]"<<std::endl;
            }
            else
            {
                // std::cout<<"Потоку "<<th_source<<" разрешение не дано. Поток направлен в очередь"<<std::endl;
                turn.push_back(th_source);
                // std::cout<<"Очередь потоков:"<<std::endl;
                // std::cout<<"Turn = [";
                // for(int t:turn)
                // std::cout<<t<<" ";
                // std::cout<<"]"<<std::endl;
            }
        }
        else if(flag == -1)
        {
            // std::cout<<"Поток "<<th_source<<" выходит........"<<std::endl;
            send_permission(forks,th_source,philosoph_num,false);

            // std::cout<<"Значения вилок"<<std::endl;
            // std::cout<<"Forks = [";
            // for(int i =0;i<philosoph_num;i++)
            // std::cout<<forks[i]<<" ";
            // std::cout<<"]"<<std::endl;

            // std::cout<<"Смотрим очередь..........."<<std::endl;
            // std::cout<<"Очередь потоков:"<<std::endl;
            // std::cout<<"Turn = [";
            // for(int t:turn)
            // std::cout<<t<<" ";
            // std::cout<<"]"<<std::endl;


            std::list<int> enabled_treads;
            
            for(int ph:turn)
            {
                if(is_can_eat(forks,ph,philosoph_num))
                {
                    //std::cout<<"Поток "<<ph<<" может выйти из очереди"<<std::endl;
                    send_permission(forks,ph,philosoph_num,true);
                    // std::cout<<"Поток "<<ph<<" может выйти из очереди"<<std::endl;
                    n = critical_code(n, ph);
                    // std::cout<<"Критическая секция поток"<<ph<<std::endl;
                    // std::cout<<"N = "<<n<<std::endl;
                    enabled_treads.push_back(ph);

                    // std::cout<<"Значения вилок"<<std::endl;
                    // std::cout<<"Forks = [";
                    // for(int i =0;i<philosoph_num;i++)
                    // std::cout<<forks[i]<<" ";
                    // std::cout<<"]"<<std::endl;
                    break;
                }
            }
            for(int enabled_tread:enabled_treads)
                turn.remove(enabled_tread);

            // std::cout<<"Смотрим очередь..........."<<std::endl;
            // std::cout<<"Очередь потоков:"<<std::endl;
            // std::cout<<"Turn = [";
            // for(int t:turn)
            // std::cout<<t<<" ";
            // std::cout<<"]"<<std::endl;
            // std::cout<<"I = "<<i<<std::endl;

            // std::cout<<(bool)(i<philosoph_num || !turn.empty()||!is_cache_free(forks,philosoph_num))<<std::endl;

        }

    // std::cout<<"__________________________________________________________________\n"<<std::endl;
    }while(i<philosoph_num || !turn.empty()||!is_cache_free(forks,philosoph_num));
    // std::cout<<"Не может вернуть"<<std::endl;
    return n;
}

