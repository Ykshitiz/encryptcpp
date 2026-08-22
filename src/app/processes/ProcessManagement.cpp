#include "ProcessManagement.hpp"
#include<iostream>
#include<cstring>
#include "../encryptDecrypt/Cryption.hpp"
// #include<sys/wait.h>

ProcessManagement::ProcessManagement(){}

bool ProcessManagement::submitToQueue(std::unique_ptr<Task> task){
    tasksQueue.push(std::move(task));
    return true;
}


void ProcessManagement::executeTasks(){
    while(!tasksQueue.empty()){
        std::unique_ptr<Task> taskToExecute = std::move(tasksQueue.front());
        tasksQueue.pop();
        std::cout<<"Executing task:"<<taskToExecute->toString()<<std::endl;
        executeCryption(taskToExecute->toString());
    }
}