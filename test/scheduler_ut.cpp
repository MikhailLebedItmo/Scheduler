#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <any>


#include "../lib/any.h"
#include "../lib/scheduler.h"


TEST(SimpleChain, 1) {
    double a = 1.5;
    double b = 2.5;

    Scheduler scheduler;

    auto id1 = scheduler.add([](double a, double b) { return a + b; }, a, b);

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<double>(id1), 4.0);
}

TEST(SimpleChain, 2) {
    double a = 1.5;
    double b = 2.5;
    double c = 2.0;

    Scheduler scheduler;

    auto id1 = scheduler.add([](double a, double b) { return a + b; }, a, b);

    auto id2 = scheduler.add([](double a, double b) { return a - b; }, scheduler.get_future_result<double>(id1), c);

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<double>(id1), 4.0);
    ASSERT_EQ(scheduler.get_result<double>(id2), 2.0);
}

TEST(SquareFootage, 1) {
    float a = 1;
    float b = -2;
    float c = 0;

    Scheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.get_future_result<float>(id1));

    auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.get_future_result<float>(id2));

    auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.get_future_result<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.get_future_result<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.get_future_result<float>(id4));

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<float>(id5), 2.0);
    ASSERT_EQ(scheduler.get_result<float>(id6), 0.0);
}
