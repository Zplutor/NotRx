#pragma once

#include <map>
#include <mutex>
#include <vector>
#include "rx/internal/subscription.h"
#include "rx/internal/subscription_core.h"

namespace rx {
namespace internal {

class SubscriptionContainer {
public:
    SubscriptionContainer() : state_(std::make_shared<State>()) { }

    SubscriptionContainer(const SubscriptionContainer&) = delete;
    SubscriptionContainer& operator=(const SubscriptionContainer&) = delete;

    ~SubscriptionContainer() {
        Clear();
    }

    void Add(const std::shared_ptr<Subscription>& subscription) {

        auto core = subscription->GetCore();
        if (core == nullptr) {
            return;
        }

        auto register_result = core->RegisterFinishCallback(std::bind(OnNoIdSubscriptionFinish, state_, std::placeholders::_1));
        if (!register_result.first) {
            return;
        }

        std::scoped_lock<std::mutex> lock(state_->lock);
        state_->no_id_items.push_back({ subscription, register_result.second });
    }

    void Add(const std::string& id, const std::shared_ptr<Subscription>& subscription) {

        auto core = subscription->GetCore();
        if (core == nullptr) {
            return;
        }

        auto register_result = core->RegisterFinishCallback(std::bind(OnIdSubscriptionFinish, state_, std::placeholders::_1));
        if (!register_result.first) {
            return;
        }

        Item item{ subscription, register_result.second };

        std::scoped_lock<std::mutex> lock(state_->lock);
        auto iterator = state_->id_items.find(id);
        if (iterator == state_->id_items.end()) {
            state_->id_items.insert(std::make_pair(id, item));
        }
        else {
            UnregisterItemCallback(iterator->second);
            iterator->second = item;
        }
    }

    void Remove(const std::string& id) {

        std::scoped_lock<std::mutex> lock(state_->lock);

        auto iterator = state_->id_items.find(id);
        if (iterator != state_->id_items.end()) {

            UnregisterItemCallback(iterator->second);
            state_->id_items.erase(iterator);
        }
    }

    void Clear() {

        std::scoped_lock<std::mutex> lock(state_->lock);

        for (const auto& each_pair : state_->id_items) {
            UnregisterItemCallback(each_pair.second);
        }
        state_->id_items.clear();

        for (const auto& each_item : state_->no_id_items) {
            UnregisterItemCallback(each_item);
        }
        state_->no_id_items.clear();
    }

private:
    class Item {
    public:
        std::shared_ptr<Subscription> subscription;
        int finish_callback_id{};
    };

    class State {
    public:
        std::map<std::string, Item> id_items;
        std::vector<Item> no_id_items;
        std::mutex lock;
    };

private:
    static void UnregisterItemCallback(const Item& item) {
        item.subscription->GetCore()->UnregisterFinishCallback(item.finish_callback_id);
    }

    static void OnNoIdSubscriptionFinish(const std::shared_ptr<State>& state, int callback_id) {

        std::scoped_lock<std::mutex> lock(state->lock);

        auto iterator = state->no_id_items.begin();
        while (iterator != state->no_id_items.end()) {

            if (iterator->finish_callback_id = callback_id) {
                iterator = state->no_id_items.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
    }

    static void OnIdSubscriptionFinish(const std::shared_ptr<State>& state, int callback_id) {

        std::scoped_lock<std::mutex> lock(state->lock);

        auto iterator = state->id_items.begin();
        while (iterator != state->id_items.end()) {

            if (iterator->second.finish_callback_id == callback_id) {
                iterator = state->id_items.erase(iterator);
            }
            else {
                ++iterator;
            }
        }
    }

private:
    std::shared_ptr<State> state_;
};

}
}