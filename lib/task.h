#pragma once
#include "any.h"


class BaseTask {
public:
    virtual void execute() = 0;

    virtual Any& get_result() = 0;

    virtual ~BaseTask() = default;
};

template <typename Func, typename FirstArg, typename SecondArg>
class Task: public BaseTask {
public:
    Task(Func func, FirstArg first_arg, SecondArg second_arg)
      : first_arg(std::move(first_arg))
      , second_arg(std::move(second_arg))
      , result()
      , func(func) {}

    void execute() override {
        if (is_completed()) {
            return;
        }
        result = func(std::move(first_arg), std::move(second_arg));
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
    FirstArg first_arg;
    SecondArg second_arg;
    Any result;
    [[no_unique_address]] Func func;
};


