#pragma once
#include "task.h"
#include "future_result.h"
#include "any.h"
#include "task_id.h"

#include <deque>
#include <memory>
#include <iostream>


class Scheduler {
public:
    Scheduler() = default;

    Scheduler(const Scheduler& other) = delete;

    Scheduler& operator=(const Scheduler& other) = delete;
public:
    template<typename Func, typename FirstArg, typename SecondArg>
    TaskId add(Func func, FirstArg first_arg, SecondArg second_arg) {
        tasks.emplace_back(new Task(func, std::move(first_arg), std::move(second_arg)));
        return TaskId(tasks.size() - 1);
    }

    template<typename ResType>
    FutureResult<ResType> get_future_result(TaskId task_id) {
        return FutureResult<ResType>(*tasks[task_id.pos]);
    }

    template<typename ResType>
    ResType& get_result(TaskId task_id) {
        return any_cast<ResType&>(tasks[task_id.pos]->get_result());
    }

    void execute_all() {
        for (auto& task: tasks) {
            task->execute();
        }
    }
private:
    std::deque<std::unique_ptr<BaseTask>> tasks;
};