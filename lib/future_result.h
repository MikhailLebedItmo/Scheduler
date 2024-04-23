#pragma once
#include "task.h"
#include "any.h"


template <typename T>
class FutureResult {
public:
    explicit FutureResult(BaseTask& task): task(task) {}

    operator T&() {
        return any_cast<T&>(task.get_result());
    }
private:
    BaseTask& task;
};
