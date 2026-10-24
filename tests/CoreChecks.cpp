#include "Event.h"
#include "WorldMath.h"
#include <iostream>
#include <stdexcept>

static void check(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}
static bool near(const glm::vec3& first, const glm::vec3& second)
{
    return glm::length(first - second) < 0.001f;
}
int main()
{
    try
    {
        Event<int> event;
        int total = 0;
        const auto first = event.Subscribe([&](int value) { total += value; });
        const auto second = event.Subscribe([&](int value) { total += value * 10; });
        event.Invoke(2);
        check(total == 22, "Event callbacks did not run");
        event.Unsubscribe(first);
        event.Invoke(1);
        check(total == 32, "Unsubscribe did not remove exactly one handler");
        event.Clear();
        const auto third = event.Subscribe([&](int value) { total += value * 100; });
        check(third != first && third != second, "Clear reused a stale subscription ID");
        event.Unsubscribe(second);
        event.Invoke(1);
        check(total == 132, "Stale ID removed a new handler");
        event.Clear();
        event.Invoke(50);
        check(total == 132, "Clear did not clear handlers");

        Event<> mutableEvent;
        Event<>::Id removed = 0;
        int calls = 0;
        mutableEvent.Subscribe([&] { ++calls; mutableEvent.Unsubscribe(removed); });
        removed = mutableEvent.Subscribe([&] { calls += 10; });
        mutableEvent.Invoke();
        check(calls == 1, "Dispatch called a handler removed during dispatch");
        mutableEvent.Clear();
        mutableEvent.Subscribe([&] { ++calls; mutableEvent.Clear(); });
        mutableEvent.Subscribe([&] { calls += 10; });
        mutableEvent.Invoke();
        check(calls == 2, "Clear during dispatch was unsafe");
        mutableEvent.Subscribe([&] { ++calls; mutableEvent.Subscribe([&] { calls += 100; }); });
        mutableEvent.Invoke();
        check(calls == 3, "New handlers should start with the next dispatch");
        mutableEvent.Invoke();
        check(calls == 104, "New handler did not join the next dispatch");
        mutableEvent.Clear();
        int captured = 0;
        mutableEvent.Subscribe([value = 0, &captured]() mutable { captured = ++value; });
        mutableEvent.Invoke(); mutableEvent.Invoke();
        check(captured == 2, "Dispatch did not preserve mutable callback state");
        mutableEvent.Clear();
        Event<>::Id self = 0;
        self = mutableEvent.Subscribe([&] { mutableEvent.Unsubscribe(self); ++captured; });
        mutableEvent.Invoke(); mutableEvent.Invoke();
        check(captured == 3, "Self-unsubscription called a removed callback");

        check(near(wrappedDelta({5001, -5001, 30001}), {-4999, 4999, 1}), "World wrapping is inconsistent");
        const glm::quat identity{1, 0, 0, 0};
        check(near(cameraLocalPosition({2, 4, 6}, {1, 2, 3}, identity), {1, 2, 3}), "Camera translation is wrong");
        const auto yaw = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0));
        check(near(cameraLocalPosition(yaw * glm::vec3(0, 0, 100), {}, yaw), {0, 0, 100}), "Inverse quaternion camera transform is wrong");
        const auto center = projectToScreen({0, 0, 100});
        const auto right = projectToScreen({100, 0, 100});
        const auto below = projectToScreen({0, 100, 100});
        check(center.x == 0 && center.y == 0 && right.x > 0 && below.y > 0, "Projection axes changed");
        check(projectToScreen({100, 0, 200}).x < right.x, "Depth does not reduce projected displacement");
        check(std::isfinite(projectToScreen({1, 1, 0}).x), "Near-plane projection is not finite");
        std::cout << "Event mutation/unsubscription and camera/wrapping checks passed.\n";
    }
    catch (const std::exception& error)
    {
        std::cerr << error.what() << '\n';
        return 1;
    }
}
