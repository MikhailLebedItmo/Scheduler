#include <vector>
#include <string>
#include <iostream>
#include "../lib/any.h"
#include "../lib/scheduler.h"
#include <cmath>
#include <any>
#include <tuple>


template <typename T>
void f(T x, T y) {
    std::cout << "first\n";
}

template <typename T, typename U>
void f(T x, U y) {
    std::cout << "second\n";
}


//template <typename ...Args>
//struct Tuple {};
//
//template <typename Head, typename ...Args>
//struct Tuple<Head, Args...> {
//    Tuple(Head&& head, Args&&... args): head(std::forward<Head>(head)), tail(Tuple(std::forward<Args>(args)...)) {}
//
//    Head head;
//    Tuple<Args...> tail;
//};
//
//template <>
//struct Tuple<> {
//
//};
//
//
//
//
//template <typename Func, typename ...TupleArgs, typename ...FuncArgs>
//auto apply(Func func, Tuple<TupleArgs...> t, FuncArgs... func_args) {
//    return apply(func, t.tail, t.head, func_args...);
//}
//
//template <typename Func, typename ...FuncArgs>
//auto apply(Func func, Tuple<> t, FuncArgs... func_args) {
//    return func(func_args...);
//}


struct Anyy {
    template<typename T>
    Anyy(T t) {}

    template<typename U>
    Anyy& operator=(const U& other) {}
};





int main() {
    Any any = std::vector<int>(5, -1);
    any = std::string("asdfsdf");
    any = 5.0;


}
