#include <vector>
#include <string>
#include <iostream>
#include "../lib/any.h"


template <typename T, typename U>
struct S {
    S(T t, U u): t(t), u(u) {}
    T t;
    U u;
};

int main() {
    {
        Any any;
        any = 5;
        any = std::string("asdf");
        std::cout << *any_cast<std::string>(&any) << '\n';
        std::vector<int> arr{1, 2, 3, 4, 5, 6};
        any = std::move(arr);
        auto brr = *any_cast<std::vector<int>>(&any);
        brr[0] = -1;
        for (auto x: brr) {
            std::cout << "x = " << x << ' ';
        }
        std::cout << '\n';
        std::cout << arr.size() << '\n';
    }
}
