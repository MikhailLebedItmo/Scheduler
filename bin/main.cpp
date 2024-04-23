#include <vector>
#include <string>
#include <iostream>
#include "../lib/any.h"
#include "../lib/scheduler.h"
#include <cmath>
#include <any>


int main() {
//    std::any some = 1.0;
//    std::cout << std::any_cast<int>(some);
    {
//        Any any;
//        any = 5;
//        any = std::string("asdf");
//        std::cout << any_cast<std::string>(any) << '\n';
//        auto& str = any_cast<std::string&>(any);
//        str = "as";
//        std::cout << any_cast<std::string>(any) << '\n';
//        std::vector<int> arr{1, 2, 3, 4, 5, 6};
//        any = std::move(arr);
//        auto brr = any_cast<std::vector<int>>(&any);
//        brr[0] = -1;
//        for (auto x: brr) {
//            std::cout << "x = " << x << ' ';
//        }
//        std::cout << '\n';
//        std::cout << arr.size() << '\n';
    }
    float a = 1;
    float b = -2;
    float c = 0;

    Scheduler scheduler;
    std::deque<std::unique_ptr<BaseTask>> tasks;
    //tasks.emplace_back(new Task([](float a, float c){return -4 * a * c;}, a, b));

    auto id1 = scheduler.add([](float a, float c){return -4 * a * c;}, a, c);

    auto id2 = scheduler.add([](float b, float v){return b * b + v;}, b, scheduler.get_future_result<float>(id1));

    auto id3 = scheduler.add([](float b, float d){return -b + std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));

    auto id4 = scheduler.add([](float b, float d){return -b - std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));

    auto id5 = scheduler.add([](float a, float v){return v/(2*a);}, a, scheduler.get_future_result<float>(id3));

    auto id6 = scheduler.add([](float a, float v){return v/(2*a);},a, scheduler.get_future_result<float>(id4));

    scheduler.execute_all();
    scheduler.Debug();

//
    std::cout << "x1 = " << scheduler.get_result<float>(id5) << std::endl;
    std::cout << "x2 = " << scheduler.get_result<float>(id6) << std::endl;

}
