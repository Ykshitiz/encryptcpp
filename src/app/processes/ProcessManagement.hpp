#ifndef PROCESS_MANAGEMENT_HPP

#define PROCESS_MANAGEMENT_HPP

#include"Task.hpp"
#include<queue>
#include<memory>
#include<atomic>
#include<semaphore.h>
#include<mutex>
class ProcessManagement{

    sem_t*itemsSemaphore;
    sem_t*emptySlotsSemaphore;
    public:
        ProcessManagement();
        ~ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task> taks);
        void executeTasks();
    private:
        // std::queue<std::unique_ptr<Task>> tasksQueue;

        struct SharedMemory {
            std::atomic<int> size;
            char tasks[1000][256];
            int front;
            int rear;

            void printSharedMemory(){
                std::cout<<size<<std::endl;
            }
        };

        SharedMemory* sharedMem;
        int shmFd;
        const char*SHM_NAME= "/my_queue";
        std::mutex queueLock;
};












#endif