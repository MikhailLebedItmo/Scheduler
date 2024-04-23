#pragma once
#include "any.h"
#include "func_arguments.h"


class BaseTask {
public:
    virtual void execute() = 0;

    virtual Any& get_result() = 0;

    virtual ~BaseTask() = default;
};

template <typename Func, typename ...Args>
class Task: public BaseTask {
public:
    Task(Func func, Args... args)
      : arguments(std::move(args)...)
      , result()
      , func(func) {}

    Task(const Task& other) = delete;

    Task& operator=(const Task& other) = delete;

    void execute() override {
        if (is_completed()) {
            return;
        }
        result = arguments.apply(func);
    }

    Any& get_result() override {
        execute();
        return result;
    }

    bool is_completed() const {
        return !result.empty();
    }

    ~Task() override = default;
private:
    FuncArgs<Args...> arguments;
    Any result;
    [[no_unique_address]] Func func;
};


