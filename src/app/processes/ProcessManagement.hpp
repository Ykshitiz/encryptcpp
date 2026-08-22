#ifndef PROCESS_MANAGEMENT_HPP

#define PROCESS_MANAGEMENT_HPP

#include"Task.hpp"
#include<queue>
#include<memory>

class ProcessManagement{
    public:
        ProcessManagement();
        bool submitToQueue(std::unique_ptr<Task> taks);
        void executeTasks();
    private:
        std::queue<std::unique_ptr<Task>> tasksQueue;
};












#endif