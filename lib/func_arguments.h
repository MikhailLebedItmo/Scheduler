#pragma once

#include <utility>

template <typename ...Args>
class FuncArgs {};

template <typename FirstArg, typename SecondArg>
class FuncArgs<FirstArg, SecondArg> {
public:
    FuncArgs(FirstArg first_arg, SecondArg second_arg)
      : first_arg(std::move(first_arg))
      , second_arg(std::move(second_arg)) {}

    template<typename Func>
    auto apply(Func func) {
        return func(std::move(first_arg), std::move(second_arg));
    }
private:
    FirstArg first_arg;
    SecondArg second_arg;
};

template <typename Arg>
class FuncArgs<Arg> {
public:
    FuncArgs(Arg arg)
            : arg(std::move(arg)) {}

    template<typename Func>
    auto apply(Func func) {
        return func(std::move(arg));
    }
private:
    Arg arg;
};

template <>
class FuncArgs<> {
public:
    template<typename Func>
    auto apply(Func func) {
        return func();
    }
};