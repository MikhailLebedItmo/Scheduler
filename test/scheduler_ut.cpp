#include "gtest/gtest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <any>


#include "../lib/any.h"
#include "../lib/scheduler.h"

TEST(SchedulerTest, 1) {
    Scheduler scheduler;
    float a = 1;
    float b = -2;
    float c = 0;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
    auto fr1 = scheduler.get_future_result<float>(id1);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, fr1);

    auto id3 =
            scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.get_future_result<float>(id2));

    auto id4 =
            scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.get_future_result<float>(id2));

    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.get_future_result<float>(id3));

    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.get_future_result<float>(id4));

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<float>(id1), 0);
    ASSERT_EQ(scheduler.get_result<float>(id2), 4);
    ASSERT_EQ(scheduler.get_result<float>(id3), 4);
    ASSERT_EQ(scheduler.get_result<float>(id4), 0);
    ASSERT_EQ(scheduler.get_result<float>(id5), 2);
    ASSERT_EQ(scheduler.get_result<float>(id6), 0);
}

TEST(SchedulerTest, 2) {
    Scheduler scheduler;

    double a = 1.5;
    double b = 2.5;

    auto id1 = scheduler.add([](double a, double b) { return a + b; }, a, b);

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<double>(id1), 4.0);
}

TEST(SchedulerTest, 3) {
    Scheduler scheduler;
    float a = 1;
    float b = -2;
    float c = 0;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
    auto fr1 = scheduler.get_future_result<float>(id1);

    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, fr1);

    auto id3 = scheduler.add([](float b, float d) {
        return -b + std::sqrt(d);}, b, scheduler.get_future_result<float>(id2));

    auto id4 =
            scheduler.add([](float b, float d) {
                return -b - std::sqrt(d); }, b, scheduler.get_future_result<float>(id2));

    auto id5 = scheduler.add([](float a, float v) {
        return v / (2 * a); }, a, scheduler.get_future_result<float>(id3));

    auto id6 = scheduler.add([](float a, float v) {
        return v / (2 * a); }, a, scheduler.get_future_result<float>(id4));

    ASSERT_EQ(scheduler.get_result<float>(id4), 0);
    ASSERT_EQ(scheduler.get_result<float>(id5), 2);
    ASSERT_EQ(scheduler.get_result<float>(id2), 4);
    ASSERT_EQ(scheduler.get_result<float>(id6), 0);
    ASSERT_EQ(scheduler.get_result<float>(id3), 4);
    ASSERT_EQ(scheduler.get_result<float>(id1), 0);
}

TEST(SchedulerTest, 4) {
    Scheduler scheduler;
    double a = 1.5;
    double b = 2.5;
    double c = 2.0;

    auto id1 = scheduler.add([](double a, double b) { return a + b; }, a, b);

    auto id2 = scheduler.add([](double a, double b) { return a - b; }, scheduler.get_future_result<double>(id1), c);

    scheduler.execute_all();

    ASSERT_EQ(scheduler.get_result<double>(id1), 4.0);
    ASSERT_EQ(scheduler.get_result<double>(id2), 2.0);
}

TEST(SchedulerTest, 6) {
    Scheduler scheduler;
    float a = 1;
    float b = -2;
    float c = 0;

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
