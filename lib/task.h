#pragma once
#include "any.h"


class BaseTask {
public:
    virtual void execute() = 0;

    virtual Any& get_result() = 0;

    virtual ~BaseTask() = 0;
};

template <typename Func, typename FirstArg, typename SecondArg>
class Task: BaseTask {
public:
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
private:
    FirstArg first_arg;
    SecondArg second_arg;
    Any result;
    [[no_unique_address]] Func func;
};


