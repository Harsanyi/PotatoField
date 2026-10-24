#pragma once
#include <algorithm>
#include <cstdint>
#include <functional>
#include <limits>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>

template<typename... Args>
class Event
{
public:
    using Id = uint64_t;

    Id Subscribe(std::function<void(Args...)> action)
    {
        if (!action) throw std::invalid_argument("Cannot subscribe an empty callback");
        if (nextId == std::numeric_limits<Id>::max()) throw std::overflow_error("Subscription ID exhausted");
        const Id id = nextId++;
        actions.push_back({id, std::make_shared<std::function<void(Args...)>>(std::move(action))});
        return id;
    }

    void Unsubscribe(Id id)
    {
        actions.erase(std::remove_if(actions.begin(), actions.end(),
            [id](const Slot& slot) { return slot.id == id; }), actions.end());
    }

    void Invoke(Args... args)
    {
        // Snapshot IDs, not iterators: handlers may unsubscribe, clear, or add handlers during dispatch.
        std::vector<Id> ids;
        ids.reserve(actions.size());
        for (const auto& slot : actions) ids.push_back(slot.id);
        for (Id id : ids)
        {
            const auto found = std::find_if(actions.begin(), actions.end(),
                [id](const Slot& slot) { return slot.id == id; });
            if (found != actions.end())
            {
                auto callback = found->action;
                (*callback)(args...);
            }
        }
    }

    void Clear() { actions.clear(); }

private:
    // Keep the same callable alive during self-unsubscription, including its mutable captured state.
    struct Slot { Id id; std::shared_ptr<std::function<void(Args...)>> action; };
    std::vector<Slot> actions;
    Id nextId = 1;
};
