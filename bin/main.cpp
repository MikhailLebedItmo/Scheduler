#include <vector>
#include <string>
#include <iostream>
#include "../lib/any.h"
#include "../lib/scheduler.h"
#include <cmath>

int main() {
    float a = 1;
    float b = -2;
    float c = 0;

    Scheduler scheduler;

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.get_future_result<float>(id1));

    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));

    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));

    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.get_future_result<float>(id3));

    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.get_future_result<float>(id4));

    scheduler.execute_all();

    std::cout << "x1 = " << scheduler.get_result<float>(id5) << std::endl;
    std::cout << "x2 = " << scheduler.get_result<float>(id6) << std::endl;
}
